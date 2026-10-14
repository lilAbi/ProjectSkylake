#ifndef PROJECTSKYLAKE_FLOORNUMBER_H
#define PROJECTSKYLAKE_FLOORNUMBER_H

#include <cstdint>
#include <compare>

struct FloorNumber {
    std::int64_t value{1};
    auto         operator<=>(const FloorNumber&) const = default;
};

#endif //PROJECTSKYLAKE_FLOORNUMBER_H
