#pragma once

#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <utility>

#include <spdlog/common.h>
#include <spdlog/details/log_msg.h>
#include <spdlog/formatter.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// C++ logging service. It owns no Godot objects and starts no worker threads.
// Keep all users inside this extension; stop/join workers before unloading it.
class Logger final {
public:
    using Level = spdlog::level::level_enum;

    // Preserves existing Logger::get()->print(...) call sites. First use may
    // allocate and throw, so get() deliberately is not noexcept.
    [[nodiscard]] static Logger* get() {
        static Logger instance;
        return &instance;
    }

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    // Resume after stop_logging(), including a module reinitialization in the
    // same loaded library. Sinks are constructed only once.
    void start_logging() {
        std::unique_lock lock(m_state_mutex);
        m_stopped = false;
    }

    // Wait for accepted log calls to finish, reject subsequent calls, then
    // flush. Safe to call repeatedly. This does not stop producer threads.
    void stop_logging() {
        std::unique_lock lock(m_state_mutex);
        if (m_stopped)
            return;
        m_stopped = true;
        m_logger.flush();
    }

    void flush() {
        std::shared_lock lock(m_state_mutex);
        m_logger.flush();
    }

    // spdlog stores the logger level atomically. This never resumes a stopped
    // logger; start_logging() is the explicit lifecycle operation.
    void set_level(Level level) {
        std::shared_lock lock(m_state_mutex);
        m_logger.set_level(level);
    }

    [[nodiscard]] bool should_log(Level level) const {
        std::shared_lock lock(m_state_mutex);
        return !m_stopped && level != spdlog::level::off && m_logger.should_log(level);
    }

    template <typename... TArgs>
    void log(Level level, spdlog::format_string_t<TArgs...> format_str, TArgs&&... args) {
        // The lock spans formatting and synchronous output, so stop_logging()
        // cannot return while an accepted call is still writing.
        std::shared_lock lock(m_state_mutex);
        if (!m_stopped && level != spdlog::level::off) {
            m_logger.log(level, format_str, std::forward<TArgs>(args)...);
        }
    }

    template <typename... TArgs>
    void print(spdlog::format_string_t<TArgs...> format_str, TArgs&&... args) {
        log(spdlog::level::info, format_str, std::forward<TArgs>(args)...);
    }

    template <typename... TArgs>
    void trace(spdlog::format_string_t<TArgs...> format_str, TArgs&&... args) {
        log(spdlog::level::trace, format_str, std::forward<TArgs>(args)...);
    }

    template <typename... TArgs>
    void debug(spdlog::format_string_t<TArgs...> format_str, TArgs&&... args) {
        log(spdlog::level::debug, format_str, std::forward<TArgs>(args)...);
    }

    template <typename... TArgs>
    void info(spdlog::format_string_t<TArgs...> format_str, TArgs&&... args) {
        log(spdlog::level::info, format_str, std::forward<TArgs>(args)...);
    }

    template <typename... TArgs>
    void warn(spdlog::format_string_t<TArgs...> format_str, TArgs&&... args) {
        log(spdlog::level::warn, format_str, std::forward<TArgs>(args)...);
    }

    template <typename... TArgs>
    void error(spdlog::format_string_t<TArgs...> format_str, TArgs&&... args) {
        log(spdlog::level::err, format_str, std::forward<TArgs>(args)...);
    }

    template <typename... TArgs>
    void critical(spdlog::format_string_t<TArgs...> format_str, TArgs&&... args) {
        log(spdlog::level::critical, format_str, std::forward<TArgs>(args)...);
    }

private:
    // Choose exactly one destination per message. Sink minimum levels alone
    // cannot express stdout's upper bound. The contained *_mt sinks handle
    // their own synchronization, including formatter access.
    class SplitConsoleSink final : public spdlog::sinks::sink {
    public:
        void log(const spdlog::details::log_msg& message) override {
            if (message.level == spdlog::level::off) {
                return;
            }
            if (message.level < spdlog::level::err) {
                m_stdout.log(message);
            } else {
                m_stderr.log(message);
            }
        }

        void flush() override {
            m_stdout.flush();
            m_stderr.flush();
        }

        void set_pattern(const std::string& pattern) override {
            m_stdout.set_pattern(pattern);
            m_stderr.set_pattern(pattern);
        }

        void set_formatter(std::unique_ptr<spdlog::formatter> formatter) override {
            // Formatters hold mutable state; each output owns its own instance.
            m_stdout.set_formatter(formatter->clone());
            m_stderr.set_formatter(std::move(formatter));
        }

    private:
        spdlog::sinks::stdout_color_sink_mt m_stdout;
        spdlog::sinks::stderr_color_sink_mt m_stderr;
    };

    Logger() : m_logger("Project Skylake", {std::make_shared<SplitConsoleSink>()}) {
        m_logger.set_level(spdlog::level::info);
        m_logger.set_pattern("%^[%H:%M:%S.%e] [%n] [%l] [thread %t] %v%$");
        m_logger.flush_on(spdlog::level::err);
    }

    ~Logger() noexcept {
        try {
            stop_logging();
        } catch (...) {
            // Explicit teardown is the normal flush path. Destruction must
            // never throw, and must not recursively call this logger.
        }
    }

    mutable std::shared_mutex m_state_mutex;
    spdlog::logger            m_logger;
    bool                      m_stopped{false};
};
