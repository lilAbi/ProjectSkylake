#ifndef PROJECTSKYLAKE_EVENT_BUS_H
#define PROJECTSKYLAKE_EVENT_BUS_H

#include <atomic>
#include <concepts>
#include <cstddef>
#include <deque>
#include <exception>
#include <functional>
#include <limits>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <utility>

using EventTypeID = const void*;

template <typename Event>
inline constexpr std::byte g_event_type_token{};

// A process-local type key, not a persistent game/save identity.
template <typename Event>
[[nodiscard]] constexpr EventTypeID event_type_id() noexcept {
    return std::addressof(g_event_type_token<std::remove_cvref_t<Event>>);
}

struct Options {
    std::size_t max_pending_events = 4096;
};

enum class Post_result {
    posted,
    queue_full,
    shutting_down
};

struct DispatchStats {
    std::size_t m_events_dispatched = 0;
    std::size_t m_callbacks_invoked = 0;
    std::size_t m_callback_failures = 0;
    std::size_t m_remaining_events = 0;
    bool        m_wrong_thread = false;
    bool        m_reentrant_dispatch_attempt = false;
};

using ErrorHandler = std::function<void(EventTypeID, std::exception_ptr)>;

struct Connection {
    virtual ~Connection() = default;
    virtual void invoke(const void* event) = 0;

    std::atomic_bool m_connected{true};
};

template <typename Event, typename Callback>
struct CallbackModel final : Connection {
    template <typename T>
    explicit CallbackModel(T&& callback)
        : m_callback{std::forward<T>(callback)} {}

    void invoke(const void* event) override {
        std::invoke(m_callback, *static_cast<const Event*>(event));
    }

    Callback m_callback;
};

struct QueuedEvent {
    virtual ~QueuedEvent() = default;
    [[nodiscard]] virtual EventTypeID type_id() const noexcept = 0;
    [[nodiscard]] virtual const void* data() const noexcept = 0;
};

template <typename Event>
struct EventModel final : QueuedEvent {
    template <typename... Args>
    explicit EventModel(std::in_place_t, Args&&... args) : m_event{std::forward<Args>(args)...} {}

    [[nodiscard]] EventTypeID type_id() const noexcept override {
        return event_type_id<Event>();
    }

    [[nodiscard]] const void* data() const noexcept override {
        return std::addressof(m_event);
    }

    Event m_event;
};

class Subscription {
    friend class EventBus;
    explicit Subscription(const std::shared_ptr<Connection>& connection) noexcept;

public:
    Subscription() noexcept = default;
    ~Subscription();

    Subscription(Subscription&&) noexcept = default;
    Subscription& operator=(const Subscription&) = delete;

    Subscription(const Subscription&) = delete;
    Subscription& operator=(Subscription&& other) noexcept;

    explicit operator bool() const noexcept;

    void               disconnect() noexcept;
    [[nodiscard]] bool connected() const noexcept;

private:
    std::weak_ptr<Connection> m_connection;
};

class EventBus final {
    using ListenerList = std::list<std::shared_ptr<Connection>>;
    using ListenerMap = std::unordered_map<EventTypeID, ListenerList>;

public:
    explicit EventBus(Options options = {});
    ~EventBus();

    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    EventBus(EventBus&&) = delete;
    EventBus& operator=(EventBus&&) = delete;

    // The bus owns a decayed callback value: lvalues are copied; rvalues move.
    // Retain the returned handle for as long as the subscription is needed.
    template <typename Event, typename Callback>
        requires std::is_object_v<std::remove_cvref_t<Event>> &&
                 std::invocable<std::decay_t<Callback>&, const std::remove_cvref_t<Event>&> &&
                 std::constructible_from<std::decay_t<Callback>, Callback&&>
    [[nodiscard]] Subscription subscribe(Callback&& callback);

    template <typename Event>
        requires std::is_object_v<std::remove_cvref_t<Event>> &&
                 std::constructible_from<std::remove_cvref_t<Event>, Event&&>
    [[nodiscard]] Post_result post(Event&& event);

    template <typename Event, typename... Args>
        requires std::is_object_v<std::remove_cvref_t<Event>> && requires(Args&&... args) { std::remove_cvref_t<Event>{std::forward<Args>(args)...}; }
    [[nodiscard]] Post_result emplace(Args&&... args);

    // Owner thread only. At most the queue length captured at entry is drained.
    // New posts are deferred to the next call, keeping feedback loops bounded.
    [[nodiscard]] DispatchStats dispatch_pending(std::size_t max_events = std::numeric_limits<std::size_t>::max());

    // Owner thread only, outside dispatch, while open. A handler can throw; that
    // propagates just like a callback exception with no handler installed.
    [[nodiscard]] bool        set_error_handler(ErrorHandler handler);
    [[nodiscard]] bool        is_owner_thread() const noexcept;
    [[nodiscard]] bool        is_shutting_down() const noexcept;
    [[nodiscard]] std::size_t pending_count() const;

    // Stops acceptance and discards pending work. Does not wait for a callback
    // already selected on the owner thread. Keep the bus alive during all calls.
    void shutdown() noexcept;

private:
    [[nodiscard]] Subscription add_listener(EventTypeID type, std::shared_ptr<Connection> connection);
    [[nodiscard]] Post_result  enqueue(std::unique_ptr<QueuedEvent> event);
    void                       prune_disconnected();

    Options                                  m_options;
    const std::thread::id                    m_owner_thread;
    std::atomic_bool                         m_shutting_down{false};
    mutable std::mutex                       m_mutex;
    std::deque<std::unique_ptr<QueuedEvent>> m_queue;
    ListenerMap                              m_listeners;
    std::shared_ptr<const ErrorHandler>      m_error_handler;       // Protected by m_mutex.
    bool                                     m_dispatching = false; // Owner-thread access only.
};

template <typename Event, typename Callback>
    requires std::is_object_v<std::remove_cvref_t<Event>> && std::invocable<std::decay_t<Callback>&, const std::remove_cvref_t<Event>&> && std::constructible_from<std::decay_t<Callback>, Callback&&>
Subscription EventBus::subscribe(Callback&& callback) {
    using EventType = std::remove_cvref_t<Event>;
    using CallbackType = std::decay_t<Callback>;

    if (is_shutting_down())
        return {};

    // Construct user-owned state before acquiring an internal mutex.
    auto connection = std::make_shared<CallbackModel<EventType, CallbackType>>(std::forward<Callback>(callback));

    return add_listener(event_type_id<EventType>(), std::move(connection));
}

template <typename Event>
    requires std::is_object_v<std::remove_cvref_t<Event>> && std::constructible_from<std::remove_cvref_t<Event>, Event&&>
Post_result EventBus::post(Event&& event) {
    using EventType = std::remove_cvref_t<Event>;
    static_assert(std::is_nothrow_destructible_v<EventType>, "Queued event destructors must not throw");

    if (is_shutting_down())
        return Post_result::shutting_down;

    auto queued = std::make_unique<EventModel<EventType>>(
        std::in_place, std::forward<Event>(event)
    );
    return enqueue(std::move(queued));
}

template <typename Event, typename... Args>
    requires std::is_object_v<std::remove_cvref_t<Event>> && requires(Args&&... args) { std::remove_cvref_t<Event>{std::forward<Args>(args)...}; }
Post_result EventBus::emplace(Args&&... args) {
    using EventType = std::remove_cvref_t<Event>;
    static_assert(std::is_nothrow_destructible_v<EventType>, "Queued event destructors must not throw");

    if (is_shutting_down())
        return Post_result::shutting_down;

    auto queued = std::make_unique<EventModel<EventType>>(std::in_place, std::forward<Args>(args)...);

    return enqueue(std::move(queued));
}

#endif // PROJECTSKYLAKE_EVENT_BUS_H
