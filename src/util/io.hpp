#pragma once

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include <fmt/compile.h>
#include <fmt/core.h>
#include <fmt/format-inl.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <godot_cpp/core/error_macros.hpp>

#include "concepts.hpp"

namespace core::inline utils {
    namespace io {
        static inline auto white(auto&& val) {
            return fmt::format(FMT_COMPILE("[color=white]{}[/color]"), std::forward<decltype(val)>(val));
        }

        static inline auto gray(auto&& val) {
            return fmt::format(FMT_COMPILE("[color=gray]{}[/color]"), std::forward<decltype(val)>(val));
        }

        static inline auto black(auto&& val) {
            return fmt::format(FMT_COMPILE("[color=black]{}[/color]"), std::forward<decltype(val)>(val));
        }

        static inline auto red(auto&& val) {
            return fmt::format(FMT_COMPILE("[color=red]{}[/color]"), std::forward<decltype(val)>(val));
        }

        static inline auto orange(auto&& val) {
            return fmt::format(FMT_COMPILE("[color=orange]{}[/color]"), std::forward<decltype(val)>(val));
        }

        static inline auto yellow(auto&& val) {
            return fmt::format(FMT_COMPILE("[color=yellow]{}[/color]"), std::forward<decltype(val)>(val));
        }

        static inline auto green(auto&& val) {
            return fmt::format(FMT_COMPILE("[color=green]{}[/color]"), std::forward<decltype(val)>(val));
        }

        static inline auto blue(auto&& val) {
            return fmt::format(FMT_COMPILE("[color=blue]{}[/color]"), std::forward<decltype(val)>(val));
        }

        static inline auto purple(auto&& val) {
            return fmt::format(FMT_COMPILE("[color=purple]{}[/color]"), std::forward<decltype(val)>(val));
        }

    }

    struct Log {
        enum Log_level : uint_fast8_t {
            none,
            error,
            warning,
            info,
            debug,
            trace
        };

        static constexpr inline bool is_level_active(const Log::Log_level level) {
            return level <= Log::m_log_level;
        }

        template <typename T>
        static inline void log_error(T msg) {
            if constexpr (Log::is_level_active(Log::Log_level::error))
                ERR_PRINT_ED(std::forward<T>(msg));
        }

        template <typename T>
        static inline void log_warning(T msg) {
            if constexpr (Log::is_level_active(Log::Log_level::warning))
                WARN_PRINT_ED(std::forward<T>(msg));
        }

        template <typename T>
        static inline void log_info(T msg) {
            if constexpr (Log::is_level_active(Log::Log_level::info))
                WARN_PRINT_ED(std::forward<T>(msg));
        }

        template <typename T>
        static inline void log_trace(T msg) {
            if constexpr (Log::is_level_active(Log::Log_level::trace))
                WARN_PRINT_ED(std::forward<T>(msg));
        }

        template <typename... Args>
        static inline void print(spdlog::format_string_t<Args...> fmt, Args&&... args) {
            std::string msg{fmt::format(fmt, std::forward<Args>(args)...)};
            ERR_PRINT_ED(msg.data());
        }

        static constexpr const Log_level m_log_level{Log::Log_level::debug};
    };
}
