#ifndef PROJECTSKYLAKE_STATS_H
#define PROJECTSKYLAKE_STATS_H

#include "game/core/results.h"

#include <array>
#include <cstddef>
#include <string>
#include <cstdint>
#include <span>

enum class Stat_type : std::size_t {
    strength,
    agility,
    vitality,
    intelligence,
    max_hp,
    attack,
    defense,
    speed,
    stat_type_count
};

struct StatBlock {
    std::array<std::int64_t, static_cast<std::size_t>(Stat_type::stat_type_count)> m_values{};
    std::int64_t                                                                   get(Stat_type stat) const;
};

enum class Modifier_operation {
    flat_add,
    additive_percent,
    scale,
    base_override
};

struct Modifier_source {
    std::string m_kind;
    std::string m_stable_key;
};

struct StatModifier {
    Stat_type          m_stat;
    Modifier_operation m_operation;
    std::int64_t       m_value{};
    std::int32_t       m_priority{};
    Modifier_source    m_source;
};

struct StatBounds {
    StatBlock m_minimum;
    StatBlock m_maximum;
};

class StatCalculator {
public:
    Result<StatBlock> calculate(const StatBlock& base, std::span<const StatModifier>, const StatBounds&) const;
};

#endif //PROJECTSKYLAKE_STATS_H
