#pragma once

#include <chrono>
#include <string>
#include <type_traits>

#include <fmt/chrono.h>
#include <fmt/compile.h>
#include <fmt/core.h>
#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/variant/string.hpp>

template <typename TContext>
class Console : public godot::Object {
    GDCLASS(Console, godot::Object);
    using clock_t = std::chrono::high_resolution_clock;

public:
    Console() {
        m_static_inst = this;
        init_loggers();
    }

    ~Console() override {
        m_static_inst = nullptr;
    }

    static inline Console<TContext>* get() {
        return m_static_inst;
    }

    void stop_logging() {
        m_logger->flush();
        m_stop = true;
    }

    void init_loggers()
        requires std::same_as<TContext, godot::RichTextLabel> {
        using namespace std::chrono_literals;
        auto std_out_sink{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};
        auto std_error_sink{std::make_shared<spdlog::sinks::stderr_color_sink_mt>()};
        std_out_sink->set_level(spdlog::level::err);
        std_error_sink->set_level(spdlog::level::info);
        m_logger = std::make_unique<spdlog::logger>(spdlog::logger{{"Project Skylake"}, {std_out_sink, std_error_sink}});
        spdlog::flush_every(0.25s);
    }

    template <typename... TArgs>
    void print(fmt::format_string<TArgs...> format_str, TArgs&&... args) {
        m_logger->info(format_str, std::forward<TArgs>(args)...);
    }

protected:
    static void _bind_methods() {}

private:
    std::unique_ptr<spdlog::logger> m_logger{nullptr};
    std::atomic<bool>               m_stop{false};

private:
    static inline Console<TContext>* m_static_inst{nullptr};
};

using console = Console<godot::RichTextLabel>;