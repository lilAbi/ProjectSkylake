#ifndef PROJECTSKYLAKE_SEED_H
#define PROJECTSKYLAKE_SEED_H

#include <compare>
#include <cstdint>
#include <string>

struct Seed {
    std::uint64_t m_value{};
    auto          operator<=>(const Seed&) const = default;
};

struct GenerationMetadata {
    Seed          m_seed;
    std::uint32_t m_generator_version{1};
    std::string   m_content_version;
};

#endif //PROJECTSKYLAKE_SEED_H
