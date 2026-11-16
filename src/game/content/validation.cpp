#include "validation.h"

#include "catalog.h"

#include <limits>
#include <optional>
#include <set>
#include <type_traits>
#include <utility>

namespace {

    template <typename Enum>
    bool enum_below(Enum value, int count) {
        const auto number = std::to_underlying(value);
        return std::cmp_greater_equal(number, 0) && std::cmp_less(number, count);
    }

    template <typename Definition>
    std::string location(const char* kind, const Definition& definition) {
        return std::string(kind) + "/" + definition.m_id.m_value;
    }

    class Validator {
    public:
        explicit Validator(const ContentCatalog& catalog) : m_catalog(catalog) {}

        void check(bool condition, const std::string& message);

        template <typename ID>
        void reference(const ID& id, const std::string& path);

        template <typename ID>
        void references(const std::vector<ID>& ids, const std::string& path);

        void unlocks(const std::vector<UnlockID>& ids, const std::string& path);

        void cost(const ResourceCost& cost, const std::string& path);

        void costs(const std::vector<ResourceCost>& costs, const std::string& path);

        void stats(const StatBlock& stats, const std::string& path);

        void modifiers(const std::vector<StatModifier>& modifiers, const std::string& path);

        void effects(const std::vector<EffectDefinition>& effects, const std::string& path);

        void battlefield(const BattlefieldTemplate& battlefield, const std::string& path);

        template <typename Entry>
        void weights(const std::vector<Entry>& entries, const std::string& path);

        [[nodiscard]] Result<void> result() const;

    private:
        const ContentCatalog& m_catalog;
        std::optional<Error>  m_error;
    };

    template <typename ID>
    void Validator::reference(const ID& id, const std::string& path) {
        check(m_catalog.find(id) != nullptr, path + ": unknown definition '" + id.m_value + "'");
    }

    template <typename ID>
    void Validator::references(const std::vector<ID>& ids, const std::string& path) {
        for (std::size_t i = 0; i < ids.size(); ++i) {
            reference(ids[i], path + "/" + std::to_string(i));
        }
    }

    template <typename Entry>
    void Validator::weights(const std::vector<Entry>& entries, const std::string& path) {
        std::uint64_t total = 0;
        for (const auto& entry : entries) {
            check(entry.m_weight > 0, path + ": weight must be positive");
            const bool fits = entry.m_weight <= std::numeric_limits<std::uint64_t>::max() - total;
            check(fits, path + ": total weight overflows uint64");
            if (fits) {
                total += entry.m_weight;
            }
        }
    }

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

        const auto spawns = [&](const auto& positions, const std::string& spawn_path) {
            for (const auto& position : positions) {
                const bool inside = position.m_x >= 0 &&
                                    position.m_x < battlefield.m_width &&
                                    position.m_y >= 0 &&
                                    position.m_y < battlefield.m_height;
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

    void validate(const CharacterClassDefinition& definition, Validator& validator) {
        const auto path = location("classes", definition);
        validator.check(!definition.m_name.empty(), path + "/name: must not be empty");
        validator.stats(definition.m_base_stats, path + "/base_stats");
        validator.references(definition.m_starting_abilities, path + "/starting_abilities");
        validator.references(definition.m_allowed_traits, path + "/allowed_traits");
        std::int64_t previous = -1;
        for (const auto threshold : definition.m_level_thresholds) {
            validator.check(threshold >= 0 && threshold > previous, path + "/level_thresholds: must be nonnegative and strictly increasing");
            previous = threshold;
        }
    }

    void validate(const TraitDefinition& definition, Validator& validator) {
        const auto path = location("traits", definition);
        validator.check(!definition.m_name.empty(), path + "/name: must not be empty");
        validator.modifiers(definition.m_modifiers, path + "/modifiers");
        validator.references(definition.m_excludes, path + "/excludes");
        for (const auto& excluded : definition.m_excludes) {
            validator.check(excluded != definition.m_id, path + "/excludes: trait cannot exclude itself");
        }
    }

    void validate(const AbilityDefinition& definition, Validator& validator) {
        const auto path = location("abilities", definition);
        validator.check(!definition.m_name.empty(), path + "/name: must not be empty");
        validator.check(definition.m_action_cost >= 0 && definition.m_cooldown_turns >= 0, path + ": cost and cooldown must be nonnegative");
        const auto& targeting = definition.m_targeting;
        validator.check(enum_below(targeting.m_relation, 4) && enum_below(targeting.m_shape, 4), path + "/targeting: invalid relation or shape");
        validator.check(targeting.m_minimum_range >= 0 && targeting.m_maximum_range >= targeting.m_minimum_range && targeting.m_radius >= 0, path + "/targeting: invalid range or radius");
        validator.check(!definition.m_effects.empty(), path + "/effects: at least one effect is required");
        validator.effects(definition.m_effects, path + "/effects");
    }

    void validate(const StatusDefinition& definition, Validator& validator) {
        const auto path = location("statuses", definition);
        validator.check(!definition.m_name.empty(), path + "/name: must not be empty");
        validator.check(definition.m_maximum_stacks > 0, path + "/maximum_stacks: must be positive");
        validator.check(enum_below(definition.m_stack_policy, 3), path + "/stack_policy: invalid policy");
        validator.modifiers(definition.m_modifiers, path + "/modifiers");
        validator.effects(definition.m_turn_start_effects, path + "/turn_start_effects");
    }

    void validate(const ItemDefinition& definition, Validator& validator) {
        const auto path = location("items", definition);
        validator.check(!definition.m_name.empty(), path + "/name: must not be empty");
        validator.check(enum_below(definition.m_slot, static_cast<int>(Equipment_slot::max_equipment_slot_count)), path + "/slot: invalid slot");
        validator.check(definition.m_maximum_level > 0, path + "/maximum_level: must be positive");
        validator.modifiers(definition.m_modifiers, path + "/modifiers");
        validator.costs(definition.m_upgrade_costs, path + "/upgrade_costs");
    }

    void validate(const RecipeDefinition& definition, Validator& validator) {
        const auto path = location("recipes", definition);
        validator.cost(definition.m_cost, path + "/cost");
        validator.check(!definition.m_outputs.empty(), path + "/outputs: must not be empty");
        validator.references(definition.m_outputs, path + "/outputs");
        validator.unlocks(definition.m_required_unlocks, path + "/required_unlocks");
    }

    void validate(const BannerDefinition& definition, Validator& validator) {
        const auto path = location("banners", definition);
        validator.cost(definition.m_cost_per_summon, path + "/cost_per_summon");
        validator.check(!definition.m_allowed_classes.empty(), path + "/allowed_classes: must not be empty");
        validator.references(definition.m_allowed_classes, path + "/allowed_classes");
        validator.unlocks(definition.m_required_unlocks, path + "/required_unlocks");
    }

    void validate(const ResourceDefinition& definition, Validator& validator) {
        validator.check(!definition.m_name.empty(), location("resources", definition) + "/name: must not be empty");
    }

    void validate(const EnemyDefinition& definition, Validator& validator) {
        const auto path = location("enemies", definition);
        validator.reference(definition.m_class_id, path + "/class_id");
        validator.stats(definition.m_stats, path + "/stats");
        validator.references(definition.m_abilities, path + "/abilities");
    }

    void validate(const BattlefieldTemplate& definition, Validator& validator) {
        validator.battlefield(definition, location("battlefields", definition));
    }

    void validate(const ObjectiveDefinition& definition, Validator& validator) {
        const auto path = location("objectives", definition);
        validator.check(!definition.m_kind.empty(), path + "/kind: must not be empty");
        validator.check(definition.m_target_count > 0, path + "/target_count: must be positive");
    }

    void validate(const EncounterDefinition& definition, Validator& validator) {
        const auto path = location("encounters", definition);
        validator.reference(definition.m_battlefield, path + "/battlefield");
        validator.references(definition.m_enemies, path + "/enemies");
        validator.references(definition.m_objectives, path + "/objectives");
        validator.reference(definition.m_rewards, path + "/rewards");
    }

    void validate(const RegionDefinition& definition, Validator& validator) {
        const auto path = location("regions", definition);
        validator.check(definition.m_first_floor.m_value > 0 && definition.m_last_floor >= definition.m_first_floor, path + ": invalid floor range");
        validator.references(definition.m_battlefields, path + "/battlefields");
        validator.references(definition.m_enemies, path + "/enemies");
    }

    void validate(const CampBuildingDefinition& definition, Validator& validator) {
        const auto path = location("buildings", definition);
        validator.check(definition.m_maximum_level > 0, path + "/maximum_level: must be positive");
        validator.cost(definition.m_construction_cost, path + "/construction_cost");
        validator.costs(definition.m_upgrade_costs, path + "/upgrade_costs");
        validator.modifiers(definition.m_modifiers, path + "/modifiers");
        validator.unlocks(definition.m_required_unlocks, path + "/required_unlocks");
    }

    void validate(const RewardTableDefinition& definition, Validator& validator) {
        const auto path = location("rewards", definition);
        validator.check(enum_below(definition.m_policy, 2), path + "/policy: invalid policy");
        validator.check(definition.m_rolls >= 0 && definition.m_item_rolls >= 0 && definition.m_experience_per_participant >= 0, path + ": rolls and experience must be nonnegative");
        if (definition.m_policy == Reward_roll_policy::weighted_with_replacement) {
            validator.check(definition.m_rolls == 0 || !definition.m_entries.empty(), path + "/entries: weighted rolls require entries");
        }
        validator.check(definition.m_item_rolls == 0 || !definition.m_item_entries.empty(), path + "/item_entries: item rolls require entries");
        validator.weights(definition.m_entries, path + "/entries");
        validator.weights(definition.m_item_entries, path + "/item_entries");
        for (const auto& entry : definition.m_entries) {
            validator.reference(entry.m_resource, path + "/entries/resource");
            validator.check(entry.m_minimum >= 0 && entry.m_maximum >= entry.m_minimum, path + "/entries: invalid reward amount range");
        }
        for (const auto& entry : definition.m_item_entries) {
            validator.reference(entry.m_definition, path + "/item_entries/definition");
        }
    }

    void validate(const IdleActivityDefinition& definition, Validator& validator) {
        const auto path = location("idle_activities", definition);
        validator.reference(definition.m_rewards, path + "/rewards");
        validator.check(definition.m_interval.count() > 0 && definition.m_offline_cap.count() >= 0, path + ": interval must be positive and offline_cap must be nonnegative");
    }

    template <typename Definition>
    void validate_all(std::span<const Definition> definitions, Validator& validator) {
        for (const auto& definition : definitions) {
            validate(definition, validator);
        }
    }

}

Result<void> validate_catalog(const ContentCatalog& catalog) {
    Validator validator{catalog};
    validate_all(catalog.classes(), validator);
    validate_all(catalog.traits(), validator);
    validate_all(catalog.abilities(), validator);
    validate_all(catalog.statuses(), validator);
    validate_all(catalog.items(), validator);
    validate_all(catalog.recipes(), validator);
    validate_all(catalog.banners(), validator);
    validate_all(catalog.resources(), validator);
    validate_all(catalog.enemies(), validator);
    validate_all(catalog.battlefields(), validator);
    validate_all(catalog.objectives(), validator);
    validate_all(catalog.encounters(), validator);
    validate_all(catalog.regions(), validator);
    validate_all(catalog.buildings(), validator);
    validate_all(catalog.rewards(), validator);
    validate_all(catalog.idle_activities(), validator);
    return validator.result();
}
