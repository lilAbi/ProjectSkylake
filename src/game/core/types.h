#ifndef PROJECTSKYLAKE_TYPES_H
#define PROJECTSKYLAKE_TYPES_H
#include <compare>
#include <cstdint>
#include <string>

struct Revision {
    std::int64_t m_value{};
    auto         operator<=>(const Revision&) const = default;
};

struct ActivityGeneration {
    std::int64_t m_value{};
    auto         operator<=>(const ActivityGeneration&) const = default;
};

struct FloorNumber {
    std::int64_t m_value{1};
    auto         operator<=>(const FloorNumber&) const = default;
};

struct SaveSlot {
    std::int32_t m_value{};
    auto         operator<=>(const SaveSlot&) const = default;
};

struct Seed {
    std::uint64_t m_value{};
    auto          operator<=>(const Seed&) const = default;
};

struct BasisPoints {
    std::int32_t m_value{};
};

// 10,000 == 100%.
struct GridPosition {
    std::int32_t m_x{};
    std::int32_t m_y{};
    auto         operator<=>(const GridPosition&) const = default;
};

struct GenerationMetadata {
    Seed          m_seed;
    std::uint32_t m_generator_version{1};
    std::string   m_content_version;
};

#endif //PROJECTSKYLAKE_TYPES_H
