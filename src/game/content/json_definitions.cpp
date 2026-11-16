#include "json_definitions.h"

#include "catalog_data.h"
#include "json_reader.h"

#include <chrono>
#include <iterator>
#include <utility>

namespace content::json_detail {

    template <>
    struct Decoder<FloorNumber> {
        static FloorNumber read(const Json& value, const std::string& path) {
            return FloorNumber{decode<std::int64_t>(value, path)};
        }
    };

    template <>
    struct Decoder<BasisPoints> {
        static BasisPoints read(const Json& value, const std::string& path) {
            return BasisPoints{decode<std::int32_t>(value, path)};
        }
    };

    template <>
    struct Decoder<std::chrono::seconds> {
        static std::chrono::seconds read(const Json& value, const std::string& path) {
            return std::chrono::seconds{decode<std::int64_t>(value, path)};
        }
    };

    template <>
    struct Decoder<ResourceBundle> {
        static ResourceBundle read(const Json& value, const std::string& path) {
            if (!value.is_object()) {
                fail(path, "expected an object mapping resource IDs to amounts");
            }
            ResourceBundle result;
            for (const auto& [key, amount] : value.items()) {
                result.m_amounts.emplace(
                    ResourceID{key},
                    decode<std::int64_t>(amount, child_path(path, key))
                );
            }
            return result;
        }
    };

    template <>
    struct EnumNames<Stat_type> {
        inline static constexpr std::pair<std::string_view, Stat_type> values[] = {
            {"strength", Stat_type::strength},
            {"agility", Stat_type::agility},
            {"vitality", Stat_type::vitality},
            {"intelligence", Stat_type::intelligence},
            {"max_hp", Stat_type::max_hp},
            {"attack", Stat_type::attack},
            {"defense", Stat_type::defense},
            {"speed", Stat_type::speed},
        };
    };

    template <>
    struct EnumNames<Modifier_operation> {
        inline static constexpr std::pair<std::string_view, Modifier_operation> values[] = {
            {"flat_add", Modifier_operation::flat_add},
            {"additive_percent", Modifier_operation::additive_percent},
            {"scale", Modifier_operation::scale},
            {"base_override", Modifier_operation::base_override},
        };
    };

    template <>
    struct EnumNames<Damage_kind> {
        inline static constexpr std::pair<std::string_view, Damage_kind> values[] = {
            {"physical", Damage_kind::physical},
            {"fire", Damage_kind::fire},
            {"ice", Damage_kind::ice},
            {"poison", Damage_kind::poison},
        };
    };

    template <>
    struct EnumNames<Target_relation> {
        inline static constexpr std::pair<std::string_view, Target_relation> values[] = {
            {"self", Target_relation::self},
            {"ally", Target_relation::ally},
            {"enemy", Target_relation::enemy},
            {"any", Target_relation::any},
        };
    };

    template <>
    struct EnumNames<Target_shape> {
        inline static constexpr std::pair<std::string_view, Target_shape> values[] = {
            {"unit", Target_shape::unit},
            {"tile", Target_shape::tile},
            {"radius", Target_shape::radius},
            {"line", Target_shape::line},
        };
    };

    template <>
    struct EnumNames<Status_stack_policy> {
        inline static constexpr std::pair<std::string_view, Status_stack_policy> values[] = {
            {"replace", Status_stack_policy::replace},
            {"refresh_duration", Status_stack_policy::refresh_duration},
            {"stack_and_refresh", Status_stack_policy::stack_and_refresh},
        };
    };

    template <>
    struct EnumNames<Equipment_slot> {
        inline static constexpr std::pair<std::string_view, Equipment_slot> values[] = {
            {"main_hand", Equipment_slot::main_hand},
            {"off_hand", Equipment_slot::off_hand},
            {"helmet", Equipment_slot::helmet},
            {"chest", Equipment_slot::chest},
            {"gloves", Equipment_slot::gloves},
            {"pants", Equipment_slot::pants},
            {"boots", Equipment_slot::boots},
            {"ear_ring_1", Equipment_slot::ear_ring_1},
            {"ear_ring_2", Equipment_slot::ear_ring_2},
            {"accessory_1", Equipment_slot::accessory_1},
            {"accessory_2", Equipment_slot::accessory_2},
        };
    };

    template <>
    struct EnumNames<Reward_roll_policy> {
        inline static constexpr std::pair<std::string_view, Reward_roll_policy> values[] = {
            {"weighted_with_replacement", Reward_roll_policy::weighted_with_replacement},
            {"all_entries", Reward_roll_policy::all_entries},
        };
    };

    template <>
    struct Decoder<StatBlock> {
        static StatBlock read(const Json& value, const std::string& path) {
            if (!value.is_object()) {
                fail(path, "expected an object mapping stat names to integer values");
            }
            StatBlock result;
            for (const auto& [key, number] : value.items()) {
                const auto stat = decode<Stat_type>(Json(key), child_path(path, key));
                result.m_values[static_cast<std::size_t>(stat)] = decode<std::int64_t>(number, child_path(path, key));
            }
            return result;
        }
    };

    template <>
    struct Decoder<Modifier_source> {
        static Modifier_source read(const Json& value, const std::string& path) {
            ObjectReader    object{value, path};
            Modifier_source result{};
            object.required("kind", result.m_kind);
            object.required("stable_key", result.m_stable_key);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<StatModifier> {
        static StatModifier read(const Json& value, const std::string& path) {
            ObjectReader object{value, path};
            StatModifier result{};
            object.required("stat", result.m_stat);
            object.required("operation", result.m_operation);
            object.required("value", result.m_value);
            object.optional("priority", result.m_priority);
            object.optional("source", result.m_source);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<DamageEffect> {
        static DamageEffect read(const Json& value, const std::string& path) {
            ObjectReader object{value, path, {"kind"}};
            DamageEffect result{};
            object.required("damage_kind", result.m_kind);
            object.required("base_amount", result.m_base_amount);
            object.optional("scaling_stat", result.m_scaling_stat);
            object.optional("scaling", result.m_scaling);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<HealEffect> {
        static HealEffect read(const Json& value, const std::string& path) {
            ObjectReader object{value, path, {"kind"}};
            HealEffect   result{};
            object.required("base_amount", result.m_base_amount);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<ApplyStatusEffect> {
        static ApplyStatusEffect read(const Json& value, const std::string& path) {
            ObjectReader      object{value, path, {"kind"}};
            ApplyStatusEffect result{};
            object.required("status", result.m_status);
            object.optional("turns", result.m_turns);
            object.optional("chance", result.m_chance);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<MoveEffect> {
        static MoveEffect read(const Json& value, const std::string& path) {
            ObjectReader object{value, path, {"kind"}};
            MoveEffect   result{};
            object.optional("distance", result.m_distance);
            object.optional("push", result.m_push);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<EffectDefinition> {
        static EffectDefinition read(const Json& value, const std::string& path) {
            if (!value.is_object() || !value.contains("kind")) {
                fail(path, "effect must be an object with a kind field");
            }
            const auto kind = decode<std::string>(value.at("kind"), path + "/kind");
            if (kind == "damage") {
                return decode<DamageEffect>(value, path);
            }
            if (kind == "heal") {
                return decode<HealEffect>(value, path);
            }
            if (kind == "apply_status") {
                return decode<ApplyStatusEffect>(value, path);
            }
            if (kind == "move") {
                return decode<MoveEffect>(value, path);
            }
            fail(path + "/kind", "unknown effect kind '" + kind + "'");
        }
    };

    template <>
    struct Decoder<TargetingRule> {
        static TargetingRule read(const Json& value, const std::string& path) {
            ObjectReader  object{value, path};
            TargetingRule result{};
            object.required("relation", result.m_relation);
            object.required("shape", result.m_shape);
            object.optional("minimum_range", result.m_minimum_range);
            object.optional("maximum_range", result.m_maximum_range);
            object.optional("radius", result.m_radius);
            object.optional("requires_line_of_sight", result.m_requires_line_of_sight);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<GridTile> {
        static GridTile read(const Json& value, const std::string& path) {
            ObjectReader object{value, path};
            GridTile     result{};
            object.optional("walkable", result.m_walkable);
            object.optional("movement_cost", result.m_movement_cost);
            object.optional("elevation", result.m_elevation);
            object.optional("blocks_sight", result.m_blocks_sight);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<GridPosition> {
        static GridPosition read(const Json& value, const std::string& path) {
            ObjectReader object{value, path};
            GridPosition result{};
            object.required("x", result.m_x);
            object.required("y", result.m_y);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<CharacterClassDefinition> {
        static CharacterClassDefinition read(const Json& value, const std::string& path) {
            ObjectReader             object{value, path};
            CharacterClassDefinition result{};
            object.required("id", result.m_id);
            object.required("name", result.m_name);
            object.required("base_stats", result.m_base_stats);
            object.optional("starting_abilities", result.m_starting_abilities);
            object.optional("allowed_traits", result.m_allowed_traits);
            object.optional("level_thresholds", result.m_level_thresholds);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<TraitDefinition> {
        static TraitDefinition read(const Json& value, const std::string& path) {
            ObjectReader    object{value, path};
            TraitDefinition result{};
            object.required("id", result.m_id);
            object.required("name", result.m_name);
            object.optional("modifiers", result.m_modifiers);
            object.optional("excludes", result.m_excludes);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<AbilityDefinition> {
        static AbilityDefinition read(const Json& value, const std::string& path) {
            ObjectReader      object{value, path};
            AbilityDefinition result{};
            object.required("id", result.m_id);
            object.required("name", result.m_name);
            object.optional("action_cost", result.m_action_cost);
            object.optional("cooldown_turns", result.m_cooldown_turns);
            object.required("targeting", result.m_targeting);
            object.required("effects", result.m_effects);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<StatusDefinition> {
        static StatusDefinition read(const Json& value, const std::string& path) {
            ObjectReader     object{value, path};
            StatusDefinition result{};
            object.required("id", result.m_id);
            object.required("name", result.m_name);
            object.optional("maximum_stacks", result.m_maximum_stacks);
            object.optional("stack_policy", result.m_stack_policy);
            object.optional("modifiers", result.m_modifiers);
            object.optional("turn_start_effects", result.m_turn_start_effects);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<ItemDefinition> {
        static ItemDefinition read(const Json& value, const std::string& path) {
            ObjectReader   object{value, path};
            ItemDefinition result{};
            object.required("id", result.m_id);
            object.required("name", result.m_name);
            object.required("slot", result.m_slot);
            object.optional("modifiers", result.m_modifiers);
            object.optional("maximum_level", result.m_maximum_level);
            object.optional("upgrade_costs", result.m_upgrade_costs);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<RecipeDefinition> {
        static RecipeDefinition read(const Json& value, const std::string& path) {
            ObjectReader     object{value, path};
            RecipeDefinition result{};
            object.required("id", result.m_id);
            object.optional("cost", result.m_cost);
            object.required("outputs", result.m_outputs);
            object.optional("required_unlocks", result.m_required_unlocks);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<BannerDefinition> {
        static BannerDefinition read(const Json& value, const std::string& path) {
            ObjectReader     object{value, path};
            BannerDefinition result{};
            object.required("id", result.m_id);
            object.optional("cost_per_summon", result.m_cost_per_summon);
            object.required("allowed_classes", result.m_allowed_classes);
            object.optional("required_unlocks", result.m_required_unlocks);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<ResourceDefinition> {
        static ResourceDefinition read(const Json& value, const std::string& path) {
            ObjectReader       object{value, path};
            ResourceDefinition result{};
            object.required("id", result.m_id);
            object.required("name", result.m_name);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<EnemyDefinition> {
        static EnemyDefinition read(const Json& value, const std::string& path) {
            ObjectReader    object{value, path};
            EnemyDefinition result{};
            object.required("id", result.m_id);
            object.required("class_id", result.m_class_id);
            object.required("stats", result.m_stats);
            object.optional("abilities", result.m_abilities);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<BattlefieldTemplate> {
        static BattlefieldTemplate read(const Json& value, const std::string& path) {
            ObjectReader        object{value, path};
            BattlefieldTemplate result{};
            object.required("id", result.m_id);
            object.required("width", result.m_width);
            object.required("height", result.m_height);
            object.required("tiles", result.m_tiles);
            object.optional("ally_spawns", result.m_ally_spawns);
            object.optional("enemy_spawns", result.m_enemy_spawns);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<ObjectiveDefinition> {
        static ObjectiveDefinition read(const Json& value, const std::string& path) {
            ObjectReader        object{value, path};
            ObjectiveDefinition result{};
            object.required("id", result.m_id);
            object.required("kind", result.m_kind);
            object.required("target_count", result.m_target_count);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<EncounterDefinition> {
        static EncounterDefinition read(const Json& value, const std::string& path) {
            ObjectReader        object{value, path};
            EncounterDefinition result{};
            object.required("id", result.m_id);
            object.required("battlefield", result.m_battlefield);
            object.required("enemies", result.m_enemies);
            object.optional("objectives", result.m_objectives);
            object.required("rewards", result.m_rewards);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<RegionDefinition> {
        static RegionDefinition read(const Json& value, const std::string& path) {
            ObjectReader     object{value, path};
            RegionDefinition result{};
            object.required("id", result.m_id);
            object.required("first_floor", result.m_first_floor);
            object.required("last_floor", result.m_last_floor);
            object.required("battlefields", result.m_battlefields);
            object.required("enemies", result.m_enemies);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<CampBuildingDefinition> {
        static CampBuildingDefinition read(const Json& value, const std::string& path) {
            ObjectReader           object{value, path};
            CampBuildingDefinition result{};
            object.required("id", result.m_id);
            object.optional("maximum_level", result.m_maximum_level);
            object.optional("construction_cost", result.m_construction_cost);
            object.optional("upgrade_costs", result.m_upgrade_costs);
            object.optional("modifiers", result.m_modifiers);
            object.optional("required_unlocks", result.m_required_unlocks);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<RewardEntry> {
        static RewardEntry read(const Json& value, const std::string& path) {
            ObjectReader object{value, path};
            RewardEntry  result{};
            object.required("resource", result.m_resource);
            object.required("minimum", result.m_minimum);
            object.required("maximum", result.m_maximum);
            object.optional("weight", result.m_weight);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<ItemRewardEntry> {
        static ItemRewardEntry read(const Json& value, const std::string& path) {
            ObjectReader    object{value, path};
            ItemRewardEntry result{};
            object.required("definition", result.m_definition);
            object.optional("weight", result.m_weight);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<RewardTableDefinition> {
        static RewardTableDefinition read(const Json& value, const std::string& path) {
            ObjectReader          object{value, path};
            RewardTableDefinition result{};
            object.required("id", result.m_id);
            object.optional("entries", result.m_entries);
            object.optional("rolls", result.m_rolls);
            object.optional("policy", result.m_policy);
            object.optional("experience_per_participant", result.m_experience_per_participant);
            object.optional("item_entries", result.m_item_entries);
            object.optional("item_rolls", result.m_item_rolls);
            object.finish();
            return result;
        }
    };

    template <>
    struct Decoder<IdleActivityDefinition> {
        static IdleActivityDefinition read(const Json& value, const std::string& path) {
            ObjectReader           object{value, path};
            IdleActivityDefinition result{};
            object.required("id", result.m_id);
            object.required("rewards", result.m_rewards);
            object.optional("interval", result.m_interval);
            object.optional("offline_cap", result.m_offline_cap);
            object.finish();
            return result;
        }
    };

    namespace {

        template <typename T>
        void append_definitions(std::vector<T>& destination, const Json& definitions) {
            // Decode one source at a time; discard its JSON tree before reading the next.
            auto loaded = decode<std::vector<T>>(definitions, "#/definitions");
            destination.insert(destination.end(), std::make_move_iterator(loaded.begin()), std::make_move_iterator(loaded.end()));
        }

    }

    void decode_definitions(CatalogData& data, const std::string_view kind, const Json& definitions) {
        if (kind == "classes") {
            append_definitions(data.m_classes, definitions);
        } else if (kind == "traits") {
            append_definitions(data.m_traits, definitions);
        } else if (kind == "abilities") {
            append_definitions(data.m_abilities, definitions);
        } else if (kind == "statuses") {
            append_definitions(data.m_statuses, definitions);
        } else if (kind == "items") {
            append_definitions(data.m_items, definitions);
        } else if (kind == "recipes") {
            append_definitions(data.m_recipes, definitions);
        } else if (kind == "banners") {
            append_definitions(data.m_banners, definitions);
        } else if (kind == "resources") {
            append_definitions(data.m_resources, definitions);
        } else if (kind == "enemies") {
            append_definitions(data.m_enemies, definitions);
        } else if (kind == "battlefields") {
            append_definitions(data.m_battlefields, definitions);
        } else if (kind == "objectives") {
            append_definitions(data.m_objectives, definitions);
        } else if (kind == "encounters") {
            append_definitions(data.m_encounters, definitions);
        } else if (kind == "regions") {
            append_definitions(data.m_regions, definitions);
        } else if (kind == "buildings") {
            append_definitions(data.m_buildings, definitions);
        } else if (kind == "rewards") {
            append_definitions(data.m_rewards, definitions);
        } else if (kind == "idle_activities") {
            append_definitions(data.m_idle_activities, definitions);
        } else {
            fail("#/kind", "unknown definition kind '" + std::string(kind) + "'");
        }
    }

}
