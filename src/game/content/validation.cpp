#include "validation.h"

void Validator::check(const bool condition, const std::string& message) {
    if (!condition && !m_error) {
        m_error = Error{
            .m_code = Error_code::invalid_content,
            .m_message = message
        };
    }
}

void Validator::unlocks(const std::vector<UnlockID>& ids, const std::string& path) {
    // Unlocks are state flags; the domain has no UnlockDefinition registry.
    for (const auto& id : ids) {
        check(valid_definition_id(id.m_value), path + ": invalid unlock ID '" + id.m_value + "'");
    }
}

void Validator::cost(const ResourceCost& cost, const std::string& path) {
    for (const auto& [id, amount] : cost.m_amounts) {
        reference(id, path);
        check(amount >= 0, path + "/" + id.m_value + ": cost must be nonnegative");
    }
}

void Validator::costs(const std::vector<ResourceCost>& costs, const std::string& path) {
    for (std::size_t i = 0; i < costs.size(); ++i) {
        cost(costs[i], path + "/" + std::to_string(i));
    }
}

void Validator::stats(const StatBlock& stats, const std::string& path) {
    for (const auto value : stats.m_values) {
        check(value >= 0, path + ": base stats must be nonnegative");
    }
    check(stats.m_values[static_cast<std::size_t>(Stat_type::max_hp)] > 0, path + "/max_hp: must be positive");
}

void Validator::modifiers(const std::vector<StatModifier>& modifiers, const std::string& path) {
    for (const auto& modifier : modifiers) {
        check(enum_below(modifier.m_stat, static_cast<int>(Stat_type::stat_type_count)), path + ": invalid stat");
        check(enum_below(modifier.m_operation, 4), path + ": invalid modifier operation");
        if (modifier.m_operation == Modifier_operation::scale) {
            check(modifier.m_value >= 0, path + ": scale must be nonnegative");
        }
    }
}

void Validator::effects(const std::vector<EffectDefinition>& effects, const std::string& path) {
    for (std::size_t i = 0; i < effects.size(); ++i) {
        const auto effect_path = path + "/" + std::to_string(i);
        std::visit(
            [&]<typename T>(const T& effect) {
                using Effect = std::decay_t<T>;
                if constexpr (std::is_same_v<Effect, DamageEffect>) {
                    check(enum_below(effect.m_kind, 4), effect_path + ": invalid damage kind");
                    check(enum_below(effect.m_scaling_stat, static_cast<int>(Stat_type::stat_type_count)), effect_path + ": invalid scaling stat");
                    check(effect.m_base_amount >= 0 && effect.m_scaling.m_value >= 0, effect_path + ": damage and scaling must be nonnegative");
                } else if constexpr (std::is_same_v<Effect, HealEffect>) {
                    check(effect.m_base_amount >= 0, effect_path + ": healing must be nonnegative");
                } else if constexpr (std::is_same_v<Effect, ApplyStatusEffect>) {
                    reference(effect.m_status, effect_path + "/status");
                    check(effect.m_turns > 0, effect_path + "/turns: must be positive");
                    check(effect.m_chance.m_value >= 0 && effect.m_chance.m_value <= 10'000, effect_path + "/chance: must be 0-10000 basis points");
                } else if constexpr (std::is_same_v<Effect, MoveEffect>) {
                    check(effect.m_distance > 0, effect_path + "/distance: must be positive");
                }
            },
            effects[i]
        );
    }
}

void Validator::battlefield(const BattlefieldTemplate& battlefield, const std::string& path) {
    check(battlefield.m_width > 0 && battlefield.m_height > 0, path + ": dimensions must be positive");
    const auto area = static_cast<std::int64_t>(battlefield.m_width) * battlefield.m_height;
    const bool complete = area > 0 && static_cast<std::uint64_t>(area) == battlefield.m_tiles.size();
    check(complete, path + "/tiles: count must equal width * height (row-major order)");
    for (const auto& tile : battlefield.m_tiles) {
        check(tile.m_movement_cost > 0, path + "/tiles: movement_cost must be positive");
    }
    std::set<GridPosition> occupied;
    const auto             spawns = [&](const auto& positions, const std::string& spawn_path) {
        for (const auto& position : positions) {
            const bool inside = position.m_x >= 0 && position.m_x < battlefield.m_width &&
                                position.m_y >= 0 && position.m_y < battlefield.m_height;
            check(inside, spawn_path + ": spawn is outside the battlefield");
            check(occupied.insert(position).second, spawn_path + ": duplicate or overlapping spawn");
            if (inside && complete) {
                const auto index = static_cast<std::size_t>(position.m_y) * battlefield.m_width + position.m_x;
                check(battlefield.m_tiles[index].m_walkable, spawn_path + ": spawn tile must be walkable");
            }
        }
    };
    spawns(battlefield.m_ally_spawns, path + "/ally_spawns");
    spawns(battlefield.m_enemy_spawns, path + "/enemy_spawns");
}

Result<void> Validator::result() const {
    if (m_error) {
        return *m_error;
    }
    return std::monostate{};
}