#ifndef PROJECTSKYLAKE_RESULTS_H
#define PROJECTSKYLAKE_RESULTS_H

#include <string>
#include <variant>

enum class Error_code {
    invalid_argument,
    not_found,
    not_configured,
    busy,
    invalid_phase,
    insufficient_resources,
    capacity_exceeded,
    locked,
    illegal_target,
    unreachable,
    stale_revision,
    overflow,
    invalid_content,
    corrupt_save,
    unsupported_version,
    io_failure,
    cancelled,
    already_completed
};

struct Error {
    Error_code   m_code;
    std::string  m_message;
};

template <class T>
using Result = std::variant<std::conditional_t<std::is_void_v<T>, std::monostate, T>, Error>;

#endif //PROJECTSKYLAKE_RESULTS_H
