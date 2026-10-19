#include "event_bus.h"

#include <algorithm>
#include <deque>
#include <list>
#include <mutex>
#include <ranges>
#include <thread>
#include <unordered_map>
#include <vector>

class DispatchGuard final {
public:
    explicit DispatchGuard(bool& flag) noexcept : m_flag{flag} {
        m_flag = true;
    }

    ~DispatchGuard() {
        m_flag = false;
    }

    DispatchGuard(const DispatchGuard&) = delete;
    DispatchGuard& operator=(const DispatchGuard&) = delete;

private:
    bool& m_flag;
};

Subscription::Subscription(const std::shared_ptr<Connection>& connection) noexcept : m_connection{connection} {}

Subscription::~Subscription() {
    disconnect();
}

Subscription& Subscription::operator=(Subscription&& other) noexcept {
    if (this != &other) {
        disconnect();
        m_connection = std::move(other.m_connection);
    }
    return *this;
}

Subscription::operator bool() const noexcept {
    return connected();
}

void Subscription::disconnect() noexcept {
    if (const auto connection = m_connection.lock()) {
        connection->m_connected.store(false, std::memory_order_release);
    }
    // Keep the weak handle unchanged so concurrent connected()/disconnect()
    // calls can safely observe it. Moving/destruction still requires ownership.
}

bool Subscription::connected() const noexcept {
    const auto connection = m_connection.lock();
    return connection && connection->m_connected.load(std::memory_order_acquire);
}

EventBus::EventBus(const Options options) : m_options{options}, m_owner_thread{std::this_thread::get_id()} {
    if (m_options.max_pending_events == 0)
        m_options.max_pending_events = 1;
}

EventBus::~EventBus() {
    this->shutdown();
}

Subscription EventBus::add_listener(const EventTypeID type, std::shared_ptr<Connection> connection) {
    std::scoped_lock lock{m_mutex};
    if (m_shutting_down.load(std::memory_order_acquire))
        return {};

    m_listeners[type].push_back(connection);
    return Subscription{connection};
    // The lock is released before the connection parameter is destroyed.
}

Post_result EventBus::enqueue(std::unique_ptr<QueuedEvent> event) {
    std::scoped_lock lock{m_mutex};
    if (m_shutting_down.load(std::memory_order_acquire)) {
        return Post_result::shutting_down;
    }
    if (m_queue.size() >= m_options.max_pending_events) {
        return Post_result::queue_full;
    }
    m_queue.push_back(std::move(event));
    return Post_result::posted;
    // A rejected event is destroyed after the lock is released.
}

DispatchStats EventBus::dispatch_pending(std::size_t max_events) {
    DispatchStats stats{};

    if (!this->is_owner_thread()) {
        stats.m_wrong_thread = true;
        stats.m_remaining_events = this->pending_count();
        return stats;
    }

    if (m_dispatching) {
        stats.m_reentrant_dispatch_attempt = true;
        stats.m_remaining_events = this->pending_count();
        return stats;
    }

    DispatchGuard     guard{m_dispatching};
    const std::size_t budget = std::min(max_events, this->pending_count());

    this->prune_disconnected();

    while (stats.m_events_dispatched < budget) {
        std::unique_ptr<QueuedEvent>             event;
        std::vector<std::shared_ptr<Connection>> listeners;
        std::shared_ptr<const ErrorHandler>      error_handler;

        {
            std::scoped_lock lock{m_mutex};
            if (m_shutting_down.load(std::memory_order_acquire) || m_queue.empty())
                break;

            if (const auto found{m_listeners.find(m_queue.front()->type_id())}; found != m_listeners.end()) {
                // Take the per-event snapshot before popping: allocation
                // failure leaves the queued event available for a later drain.
                listeners.assign(found->second.begin(), found->second.end());
            }
            error_handler = m_error_handler;
            event = std::move(m_queue.front());
            m_queue.pop_front();
        }

        ++stats.m_events_dispatched;

        for (const auto& listener : listeners) {
            if (m_shutting_down.load(std::memory_order_acquire))
                break;

            if (!listener->m_connected.load(std::memory_order_acquire))
                continue;

            ++stats.m_callbacks_invoked;
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            try {
                listener->invoke(event->data());
            } catch (...) {
                ++stats.m_callback_failures;
                if (!error_handler)
                    throw;
                std::invoke(*error_handler, event->type_id(), std::current_exception());
                //(*error_handler)(event->type_id(), std::current_exception());
            }
#else
            listener->invoke(event->data());
#endif
        }
        // Listener snapshots and payloads are destroyed outside the lock.
    }

    this->prune_disconnected();
    stats.m_remaining_events = this->pending_count();
    return stats;
}

bool EventBus::set_error_handler(ErrorHandler handler) {
    if (!is_owner_thread() || m_dispatching || is_shutting_down())
        return false;

    auto replacement = handler ? std::make_shared<const ErrorHandler>(std::move(handler)) : nullptr;

    {
        std::scoped_lock lock{m_mutex};
        if (m_shutting_down.load(std::memory_order_acquire))
            return false;
        m_error_handler.swap(replacement);
    }

    // The replaced handler's captures are released outside the mutex.
    return true;
}

bool EventBus::is_owner_thread() const noexcept {
    return std::this_thread::get_id() == m_owner_thread;
}

bool EventBus::is_shutting_down() const noexcept {
    return m_shutting_down.load(std::memory_order_acquire);
}

std::size_t EventBus::pending_count() const {
    std::scoped_lock lock{m_mutex};
    return m_queue.size();
}

void EventBus::shutdown() noexcept {
    // Serialize shutdown with registration/enqueue and make its completed
    // deactivation visible before another shutdown call returns.
    std::deque<std::unique_ptr<QueuedEvent>> discarded_events;
    ListenerMap                              retired_listeners;
    std::shared_ptr<const ErrorHandler>      retired_error_handler;
    {
        std::scoped_lock lock{m_mutex};

        if (m_shutting_down.exchange(true, std::memory_order_acq_rel))
            return;

        for (auto& listeners : m_listeners | std::views::values) {
            for (const auto& listener : listeners) {
                listener->m_connected.store(false, std::memory_order_release);
            }
        }

        discarded_events.swap(m_queue);
        retired_listeners.swap(m_listeners);
        retired_error_handler.swap(m_error_handler);
    }
    // Destroy payloads and callbacks outside all bus locks. A dispatch snapshot
    // keeps an already selected callback alive until that dispatch releases it.
}

void EventBus::prune_disconnected() {
    {
        ListenerList     retired;
        std::scoped_lock lock{m_mutex};
        for (auto channel = m_listeners.begin(); channel != m_listeners.end();) {
            auto& listeners = channel->second;

            for (auto current = listeners.begin(); current != listeners.end();) {
                auto slot = current++;
                if (!(*slot)->m_connected.load(std::memory_order_acquire)) {
                    // Splicing does not allocate or destroy the callback.
                    retired.splice(retired.end(), listeners, slot);
                }
            }

            if (listeners.empty()) {
                channel = m_listeners.erase(channel);
            } else {
                ++channel;
            }
        }
    }
    // Captured objects can re-enter the bus from their destructors.
    // Destroy them only after releasing the mutex.
}
