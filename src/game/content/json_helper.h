#ifndef PROJECTSKYLAKE_JSON_HELPER_H
#define PROJECTSKYLAKE_JSON_HELPER_H

#include "json_content_source.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <set>
#include <string_view>
#include <type_traits>
#include <utility>

using Json = nlohmann::json;
constexpr std::size_t max_file_bytes = 16 * 1024 * 1024;
constexpr std::size_t max_total_bytes = 64 * 1024 * 1024;
constexpr std::size_t max_sources = 256;
constexpr std::size_t max_definitions = 100'000;
constexpr int         max_json_depth = 64;

[[noreturn]] inline void fail(const std::string& path, const std::string& message, Error_code code = Error_code::invalid_content) {
    throw Error{
        .m_code = code,
        .m_message = path + ": " + message
    };
}

inline std::string child_path(const std::string& path, const std::string_view key) {
    std::string result = path + "/";
    for (const char c : key) {
        result += c == '~' ? "~0" : c == '/' ? "~1"
                                             : std::string(1, c);
    }
    return result;
}

template <typename T>
struct VectorType : std::false_type {};
template <typename T>
struct VectorType<std::vector<T>> : std::true_type {
    using Element = T;
};

template <typename T>
struct DefinitionIDType : std::false_type {};
template <typename Tag>
struct DefinitionIDType<DefinitionID<Tag>> : std::true_type {};

template <typename T>
struct EnumNames;
template <typename T>
struct Decoder;

template <typename T>
T decode(const Json& value, const std::string& path) {
    //attempt to extract a string if T is a string
    if constexpr (std::is_same_v<T, std::string>) {
        if (!value.is_string()) {
            fail(path, "expected a string");
        }
        auto result = value.get<std::string>();
        if (result.find('\0') != std::string::npos) {
            fail(path, "embedded NUL is not allowed");
        }
        return result;
    } else if constexpr (std::is_same_v<T, bool>) {
        //attempt to extract a boolean if T is a bool
        if (!value.is_boolean()) {
            fail(path, "expected a boolean");
        }
        return value.get<bool>();
    } else if constexpr (std::integral<T>) {
        //attempt to extract an unsigned int if T is an integer type
        if (value.is_number_unsigned()) {
            const auto number = value.get<std::uint64_t>();
            if (!std::in_range<T>(number)) {
                fail(path, "integer is outside the destination type's range");
            }
            return static_cast<T>(number);
        }
        //attempt to extract an int if T is an integer type
        if (value.is_number_integer()) {
            const auto number = value.get<std::int64_t>();
            if (!std::in_range<T>(number)) {
                fail(path, "integer is outside the destination type's range");
            }
            return static_cast<T>(number);
        }
        fail(path, "expected an integer (floating-point values are not accepted)");
    } else if constexpr (DefinitionIDType<T>::value) {
        //attempt to extract an DefinitionID value if T is an DefinitionID type
        return T{decode<std::string>(value, path)};
    } else if constexpr (std::is_enum_v<T>) {
        //attempt to extract an enum value
        const auto name = decode<std::string>(value, path);
        for (const auto& [label, result] : EnumNames<T>::values) {
            if (name == label) {
                return result;
            }
        }
        fail(path, "unknown enum value '" + name + "'");
    } else if constexpr (VectorType<T>::value) {
        if (!value.is_array()) {
            fail(path, "expected an array");
        }
        T result;
        result.reserve(value.size());
        for (std::size_t index = 0; index < value.size(); ++index) {
            result.push_back(decode<typename VectorType<T>::Element>(value[index], child_path(path, std::to_string(index))));
        }
        return result;
    } else {
        return Decoder<T>::read(value, path);
    }
}

class Object {
public:
    Object(const Json& value, std::string path, std::initializer_list<std::string_view> allowed)
        : m_value(value), m_path(std::move(path)) {
        if (!m_value.is_object()) {
            fail(m_path, "expected an object");
        }
        for (const auto& [key, ignored] : m_value.items()) {
            if (std::ranges::find(allowed, key) == allowed.end()) {
                fail(child_path(m_path, key), "unknown field");
            }
        }
    }

    template <typename T>
    void field(std::string_view name, T& destination, bool required = false) const {
        const auto it = m_value.find(std::string{name});
        if (it == m_value.end()) {
            if (required) {
                fail(child_path(m_path, name), "missing required field");
            }
            return;
        }
        destination = decode<T>(*it, child_path(m_path, name));
    }

private:
    const Json& m_value;
    std::string m_path;
};

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
        Object          object{value, path, {"kind", "stable_key"}};
        Modifier_source result{};
        object.field("kind", result.m_kind, true);
        object.field("stable_key", result.m_stable_key, true);
        return result;
    }
};

template <>
struct Decoder<StatModifier> {
    static StatModifier read(const Json& value, const std::string& path) {
        Object       object{value, path, {"stat", "operation", "value", "priority", "source"}};
        StatModifier result{};
        object.field("stat", result.m_stat, true);
        object.field("operation", result.m_operation, true);
        object.field("value", result.m_value, true);
        object.field("priority", result.m_priority);
        object.field("source", result.m_source);
        return result;
    }
};

template <>
struct Decoder<DamageEffect> {
    static DamageEffect read(const Json& value, const std::string& path) {
        Object       object{value, path, {"kind", "damage_kind", "base_amount", "scaling_stat", "scaling"}};
        DamageEffect result{};
        object.field("damage_kind", result.m_kind, true);
        object.field("base_amount", result.m_base_amount, true);
        object.field("scaling_stat", result.m_scaling_stat);
        object.field("scaling", result.m_scaling);
        return result;
    }
};

template <>
struct Decoder<HealEffect> {
    static HealEffect read(const Json& value, const std::string& path) {
        Object     object{value, path, {"kind", "base_amount"}};
        HealEffect result{};
        object.field("base_amount", result.m_base_amount, true);
        return result;
    }
};

template <>
struct Decoder<ApplyStatusEffect> {
    static ApplyStatusEffect read(const Json& value, const std::string& path) {
        Object            object{value, path, {"kind", "status", "turns", "chance"}};
        ApplyStatusEffect result{};
        object.field("status", result.m_status, true);
        object.field("turns", result.m_turns);
        object.field("chance", result.m_chance);
        return result;
    }
};

template <>
struct Decoder<MoveEffect> {
    static MoveEffect read(const Json& value, const std::string& path) {
        Object     object{value, path, {"kind", "distance", "push"}};
        MoveEffect result{};
        object.field("distance", result.m_distance);
        object.field("push", result.m_push);
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
        Object        object{value, path, {"relation", "shape", "minimum_range", "maximum_range", "radius", "requires_line_of_sight"}};
        TargetingRule result{};
        object.field("relation", result.m_relation, true);
        object.field("shape", result.m_shape, true);
        object.field("minimum_range", result.m_minimum_range);
        object.field("maximum_range", result.m_maximum_range);
        object.field("radius", result.m_radius);
        object.field("requires_line_of_sight", result.m_requires_line_of_sight);
        return result;
    }
};

template <>
struct Decoder<GridTile> {
    static GridTile read(const Json& value, const std::string& path) {
        Object   object{value, path, {"walkable", "movement_cost", "elevation", "blocks_sight"}};
        GridTile result{};
        object.field("walkable", result.m_walkable);
        object.field("movement_cost", result.m_movement_cost);
        object.field("elevation", result.m_elevation);
        object.field("blocks_sight", result.m_blocks_sight);
        return result;
    }
};

template <>
struct Decoder<GridPosition> {
    static GridPosition read(const Json& value, const std::string& path) {
        Object       object{value, path, {"x", "y"}};
        GridPosition result{};
        object.field("x", result.m_x, true);
        object.field("y", result.m_y, true);
        return result;
    }
};

template <>
struct Decoder<CharacterClassDefinition> {
    static CharacterClassDefinition read(const Json& value, const std::string& path) {
        Object                   object{value, path, {"id", "name", "base_stats", "starting_abilities", "allowed_traits", "level_thresholds"}};
        CharacterClassDefinition result{};
        object.field("id", result.m_id, true);
        object.field("name", result.m_name, true);
        object.field("base_stats", result.m_base_stats, true);
        object.field("starting_abilities", result.m_starting_abilities);
        object.field("allowed_traits", result.m_allowed_traits);
        object.field("level_thresholds", result.m_level_thresholds);
        return result;
    }
};

template <>
struct Decoder<TraitDefinition> {
    static TraitDefinition read(const Json& value, const std::string& path) {
        Object          object{value, path, {"id", "name", "modifiers", "excludes"}};
        TraitDefinition result{};
        object.field("id", result.m_id, true);
        object.field("name", result.m_name, true);
        object.field("modifiers", result.m_modifiers);
        object.field("excludes", result.m_excludes);
        return result;
    }
};

template <>
struct Decoder<AbilityDefinition> {
    static AbilityDefinition read(const Json& value, const std::string& path) {
        Object            object{value, path, {"id", "name", "action_cost", "cooldown_turns", "targeting", "effects"}};
        AbilityDefinition result{};
        object.field("id", result.m_id, true);
        object.field("name", result.m_name, true);
        object.field("action_cost", result.m_action_cost);
        object.field("cooldown_turns", result.m_cooldown_turns);
        object.field("targeting", result.m_targeting, true);
        object.field("effects", result.m_effects, true);
        return result;
    }
};

template <>
struct Decoder<StatusDefinition> {
    static StatusDefinition read(const Json& value, const std::string& path) {
        Object           object{value, path, {"id", "name", "maximum_stacks", "stack_policy", "modifiers", "turn_start_effects"}};
        StatusDefinition result{};
        object.field("id", result.m_id, true);
        object.field("name", result.m_name, true);
        object.field("maximum_stacks", result.m_maximum_stacks);
        object.field("stack_policy", result.m_stack_policy);
        object.field("modifiers", result.m_modifiers);
        object.field("turn_start_effects", result.m_turn_start_effects);
        return result;
    }
};

template <>
struct Decoder<ItemDefinition> {
    static ItemDefinition read(const Json& value, const std::string& path) {
        Object         object{value, path, {"id", "name", "slot", "modifiers", "maximum_level", "upgrade_costs"}};
        ItemDefinition result{};
        object.field("id", result.m_id, true);
        object.field("name", result.m_name, true);
        object.field("slot", result.m_slot, true);
        object.field("modifiers", result.m_modifiers);
        object.field("maximum_level", result.m_maximum_level);
        object.field("upgrade_costs", result.m_upgrade_costs);
        return result;
    }
};

template <>
struct Decoder<RecipeDefinition> {
    static RecipeDefinition read(const Json& value, const std::string& path) {
        Object           object{value, path, {"id", "cost", "outputs", "required_unlocks"}};
        RecipeDefinition result{};
        object.field("id", result.m_id, true);
        object.field("cost", result.m_cost);
        object.field("outputs", result.m_outputs, true);
        object.field("required_unlocks", result.m_required_unlocks);
        return result;
    }
};

template <>
struct Decoder<BannerDefinition> {
    static BannerDefinition read(const Json& value, const std::string& path) {
        Object           object{value, path, {"id", "cost_per_summon", "allowed_classes", "required_unlocks"}};
        BannerDefinition result{};
        object.field("id", result.m_id, true);
        object.field("cost_per_summon", result.m_cost_per_summon);
        object.field("allowed_classes", result.m_allowed_classes, true);
        object.field("required_unlocks", result.m_required_unlocks);
        return result;
    }
};

template <>
struct Decoder<ResourceDefinition> {
    static ResourceDefinition read(const Json& value, const std::string& path) {
        Object             object{value, path, {"id", "name"}};
        ResourceDefinition result{};
        object.field("id", result.m_id, true);
        object.field("name", result.m_name, true);
        return result;
    }
};

template <>
struct Decoder<EnemyDefinition> {
    static EnemyDefinition read(const Json& value, const std::string& path) {
        Object          object{value, path, {"id", "class_id", "stats", "abilities"}};
        EnemyDefinition result{};
        object.field("id", result.m_id, true);
        object.field("class_id", result.m_class_id, true);
        object.field("stats", result.m_stats, true);
        object.field("abilities", result.m_abilities);
        return result;
    }
};

template <>
struct Decoder<BattlefieldTemplate> {
    static BattlefieldTemplate read(const Json& value, const std::string& path) {
        Object              object{value, path, {"id", "width", "height", "tiles", "ally_spawns", "enemy_spawns"}};
        BattlefieldTemplate result{};
        object.field("id", result.m_id, true);
        object.field("width", result.m_width, true);
        object.field("height", result.m_height, true);
        object.field("tiles", result.m_tiles, true);
        object.field("ally_spawns", result.m_ally_spawns);
        object.field("enemy_spawns", result.m_enemy_spawns);
        return result;
    }
};

template <>
struct Decoder<ObjectiveDefinition> {
    static ObjectiveDefinition read(const Json& value, const std::string& path) {
        Object              object{value, path, {"id", "kind", "target_count"}};
        ObjectiveDefinition result{};
        object.field("id", result.m_id, true);
        object.field("kind", result.m_kind, true);
        object.field("target_count", result.m_target_count, true);
        return result;
    }
};

template <>
struct Decoder<EncounterDefinition> {
    static EncounterDefinition read(const Json& value, const std::string& path) {
        Object              object{value, path, {"id", "battlefield", "enemies", "objectives", "rewards"}};
        EncounterDefinition result{};
        object.field("id", result.m_id, true);
        object.field("battlefield", result.m_battlefield, true);
        object.field("enemies", result.m_enemies, true);
        object.field("objectives", result.m_objectives);
        object.field("rewards", result.m_rewards, true);
        return result;
    }
};

template <>
struct Decoder<RegionDefinition> {
    static RegionDefinition read(const Json& value, const std::string& path) {
        Object           object{value, path, {"id", "first_floor", "last_floor", "battlefields", "enemies"}};
        RegionDefinition result{};
        object.field("id", result.m_id, true);
        object.field("first_floor", result.m_first_floor, true);
        object.field("last_floor", result.m_last_floor, true);
        object.field("battlefields", result.m_battlefields, true);
        object.field("enemies", result.m_enemies, true);
        return result;
    }
};

template <>
struct Decoder<CampBuildingDefinition> {
    static CampBuildingDefinition read(const Json& value, const std::string& path) {
        Object                 object{value, path, {"id", "maximum_level", "construction_cost", "upgrade_costs", "modifiers", "required_unlocks"}};
        CampBuildingDefinition result{};
        object.field("id", result.m_id, true);
        object.field("maximum_level", result.m_maximum_level);
        object.field("construction_cost", result.m_construction_cost);
        object.field("upgrade_costs", result.m_upgrade_costs);
        object.field("modifiers", result.m_modifiers);
        object.field("required_unlocks", result.m_required_unlocks);
        return result;
    }
};

template <>
struct Decoder<RewardEntry> {
    static RewardEntry read(const Json& value, const std::string& path) {
        Object      object{value, path, {"resource", "minimum", "maximum", "weight"}};
        RewardEntry result{};
        object.field("resource", result.m_resource, true);
        object.field("minimum", result.m_minimum, true);
        object.field("maximum", result.m_maximum, true);
        object.field("weight", result.m_weight);
        return result;
    }
};

template <>
struct Decoder<ItemRewardEntry> {
    static ItemRewardEntry read(const Json& value, const std::string& path) {
        Object          object{value, path, {"definition", "weight"}};
        ItemRewardEntry result{};
        object.field("definition", result.m_definition, true);
        object.field("weight", result.m_weight);
        return result;
    }
};

template <>
struct Decoder<RewardTableDefinition> {
    static RewardTableDefinition read(const Json& value, const std::string& path) {
        Object                object{value, path, {"id", "entries", "rolls", "policy", "experience_per_participant", "item_entries", "item_rolls"}};
        RewardTableDefinition result{};
        object.field("id", result.m_id, true);
        object.field("entries", result.m_entries);
        object.field("rolls", result.m_rolls);
        object.field("policy", result.m_policy);
        object.field("experience_per_participant", result.m_experience_per_participant);
        object.field("item_entries", result.m_item_entries);
        object.field("item_rolls", result.m_item_rolls);
        return result;
    }
};

template <>
struct Decoder<IdleActivityDefinition> {
    static IdleActivityDefinition read(const Json& value, const std::string& path) {
        Object                 object{value, path, {"id", "rewards", "interval", "offline_cap"}};
        IdleActivityDefinition result{};
        object.field("id", result.m_id, true);
        object.field("rewards", result.m_rewards, true);
        object.field("interval", result.m_interval);
        object.field("offline_cap", result.m_offline_cap);
        return result;
    }
};

void schema_version(const Json& value) {
    if (!value.is_object() || !value.contains("schema_version")) {
        fail("#/schema_version", "missing required field");
    }
    const auto version = decode<std::int32_t>(value.at("schema_version"), "#/schema_version");
    if (version != 1) {
        fail("#/schema_version", "unsupported schema version " + std::to_string(version), Error_code::unsupported_version);
    }
}

Json parse_json(const std::string& text) {
    std::vector<std::set<std::string>> object_keys;

    const auto callback = [&](const int depth, const Json::parse_event_t event, const Json& parsed) {
        if (depth > max_json_depth) {
            fail("#", "JSON nesting exceeds depth limit");
        }
        if (event == Json::parse_event_t::object_start) {
            object_keys.emplace_back();
        } else if (event == Json::parse_event_t::key) {
            const auto key = parsed.get<std::string>();
            if (!object_keys.back().insert(key).second) {
                fail("#", "duplicate object key '" + key + "'");
            }
        } else if (event == Json::parse_event_t::object_end) {
            object_keys.pop_back();
        }
        return true;
    };

    return Json::parse(text, callback);
}

//takes a filename listed in the catalog manifest, validates its format, then produces the path the file reader should open
std::string resolve_source(const std::string& manifest_file_path, const std::string& file_path, const std::string& json_path) {
    //check if path is empty or contains invalid characters
    if (file_path.empty() || file_path.find_first_of("\\\r\n?#") != std::string::npos) {
        fail(json_path, "source must be a nonempty, forward-slash file path");
    }
    //test if we are using a "godot" path
    const bool virtual_path = file_path.starts_with("res://") || file_path.starts_with("user://");
    //path cannot have a ":" unless its a godot prefix or cannot start with a "/"
    if ( (!virtual_path && file_path.find(':') != std::string::npos) || file_path.front() == '/' ) {
        fail(json_path, "source must be relative to the manifest or use res:// or user://");
    }
    const auto  local_path = virtual_path ? file_path.substr(file_path.find("://") + 3) : file_path;
    std::size_t start = 0;
    //examines each slash-separated component and ensure its a valid format
    while (start <= local_path.size()) {
        const auto slash = local_path.find('/', start);
        const auto part = local_path.substr(start, slash == std::string::npos ? slash : slash - start);
        if (part.empty() || part == "." || part == "..") {
            fail(json_path, "empty, '.' and '..' path components are not allowed");
        }
        if (slash == std::string::npos) {
            break;
        }
        start = slash + 1;
    }
    if (virtual_path) {
        return file_path;
    }
    //If the manifest has no slash, there is no directory prefix to prepend.
    const auto slash = manifest_file_path.rfind('/');
    return (slash == std::string::npos ? std::string{} : manifest_file_path.substr(0, slash + 1)) + file_path;
}

template <typename T>
void append_definitions(std::vector<T>& destination, const Json& definitions) {
    // Decode one source at a time; discard its JSON tree before reading the next.
    auto loaded = decode<std::vector<T>>(definitions, "#/definitions");
    destination.insert(destination.end(), std::make_move_iterator(loaded.begin()), std::make_move_iterator(loaded.end()));
}

void load_definitions(CatalogData& data, const Json& source, std::size_t& definition_count) {
    schema_version(source);
    Object      object{source, "#", {"schema_version", "kind", "definitions"}};
    std::string kind;
    object.field("kind", kind, true);
    if (!source.contains("definitions") || !source.at("definitions").is_array()) {
        fail("#/definitions", "expected an array of definitions");
    }
    const auto& definitions = source.at("definitions");
    if (definitions.size() > max_definitions - definition_count) {
        fail("#/definitions", "catalog exceeds definition limit");
    }
    definition_count += definitions.size();
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
        fail("#/kind", "unknown definition kind '" + kind + "'");
    }
}

#endif //PROJECTSKYLAKE_JSON_HELPER_H
