#ifndef PROJECTSKYLAKE_ERROR_H
#define PROJECTSKYLAKE_ERROR_H

#include <string>
#include <variant>

enum class ErrorCode {
    InvalidArgument,
    NotFound,
    NotConfigured,
    Busy,
    InvalidPhase,
    InsufficientResources,
    CapacityExceeded,
    Locked,
    IllegalTarget,
    Unreachable,
    StaleRevision,
    Overflow,
    InvalidContent,
    CorruptSave,
    UnsupportedVersion,
    IoFailure,
    Cancelled,
    AlreadyCompleted
};

struct Error {
    ErrorCode   code;
    std::string message;
};

template <class T>
using Result = std::variant<std::conditional_t<std::is_void_v<T>, std::monostate, T>, Error>;

#endif //PROJECTSKYLAKE_ERROR_H
