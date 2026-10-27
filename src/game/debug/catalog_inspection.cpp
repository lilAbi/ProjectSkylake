#include "catalog_inspection.h"

namespace debug {
    namespace {

        using Json = nlohmann::ordered_json;

        constexpr std::array categories{
            CatalogCategory{Catalog_kind::classes, "Classes", "CharacterClassDefinition"},
            CatalogCategory{Catalog_kind::traits, "Traits", "TraitDefinition"},
            CatalogCategory{Catalog_kind::abilities, "Abilities", "AbilityDefinition"},
            CatalogCategory{Catalog_kind::statuses, "Statuses", "StatusDefinition"},
            CatalogCategory{Catalog_kind::items, "Items", "ItemDefinition"},
            CatalogCategory{Catalog_kind::recipes, "Recipes", "RecipeDefinition"},
            CatalogCategory{Catalog_kind::banners, "Banners", "BannerDefinition"},
            CatalogCategory{Catalog_kind::resources, "Resources", "ResourceDefinition"},
            CatalogCategory{Catalog_kind::enemies, "Enemies", "EnemyDefinition"},
            CatalogCategory{Catalog_kind::battlefields, "Battlefields", "BattlefieldTemplate"},
            CatalogCategory{Catalog_kind::objectives, "Objectives", "ObjectiveDefinition"},
            CatalogCategory{Catalog_kind::encounters, "Encounters", "EncounterDefinition"},
            CatalogCategory{Catalog_kind::regions, "Regions", "RegionDefinition"},
            CatalogCategory{Catalog_kind::buildings, "Buildings", "CampBuildingDefinition"},
            CatalogCategory{Catalog_kind::rewards, "Rewards", "RewardTableDefinition"},
            CatalogCategory{Catalog_kind::idle_activities, "Idle activities", "IdleActivityDefinition"}
        };

        constexpr std::array stat_names{
            "strength", "agility", "vitality", "intelligence", "max_hp", "attack", "defense", "speed"
        };

        template <typename T>
        constexpr Catalog_kind kind_of();
        template <>
        constexpr Catalog_kind kind_of<ClassID>() {
            return Catalog_kind::classes;
        }
        template <>
        constexpr Catalog_kind kind_of<TraitID>() {
            return Catalog_kind::traits;
        }
        template <>
        constexpr Catalog_kind kind_of<AbilityID>() {
            return Catalog_kind::abilities;
        }
        template <>
        constexpr Catalog_kind kind_of<StatusID>() {
            return Catalog_kind::statuses;
        }
        template <>
        constexpr Catalog_kind kind_of<ItemDefinitionID>() {
            return Catalog_kind::items;
        }
        template <>
        constexpr Catalog_kind kind_of<RecipeID>() {
            return Catalog_kind::recipes;
        }
        template <>
        constexpr Catalog_kind kind_of<BannerID>() {
            return Catalog_kind::banners;
        }
        template <>
        constexpr Catalog_kind kind_of<ResourceID>() {
            return Catalog_kind::resources;
        }
        template <>
        constexpr Catalog_kind kind_of<EnemyID>() {
            return Catalog_kind::enemies;
        }
        template <>
        constexpr Catalog_kind kind_of<BattlefieldID>() {
            return Catalog_kind::battlefields;
        }
        template <>
        constexpr Catalog_kind kind_of<ObjectiveID>() {
            return Catalog_kind::objectives;
        }
        template <>
        constexpr Catalog_kind kind_of<EncounterID>() {
            return Catalog_kind::encounters;
        }
        template <>
        constexpr Catalog_kind kind_of<RegionID>() {
            return Catalog_kind::regions;
        }
        template <>
        constexpr Catalog_kind kind_of<BuildingDefinitionID>() {
            return Catalog_kind::buildings;
        }
        template <>
        constexpr Catalog_kind kind_of<RewardTableID>() {
            return Catalog_kind::rewards;
        }
        template <>
        constexpr Catalog_kind kind_of<IdleActivityID>() {
            return Catalog_kind::idle_activities;
        }

        template <typename T>
        struct IsVector : std::false_type {};
        template <typename T>
        struct IsVector<std::vector<T>> : std::true_type {};
        template <typename T>
        struct IsDefinitionID : std::false_type {};
        template <typename Tag>
        struct IsDefinitionID<DefinitionID<Tag>> : std::true_type {};

        // Separate from the file decoder: this projects the actual C++ values, including
        // defaults and zero-initialized stats. Reference metadata comes from strong ID
        // types, never from guessing whether an arbitrary string looks like an ID.
        template <typename T>
        struct Projection;

        template <typename T>
        Json project(const T& value, CatalogEntry& entry, const std::string& path) {
            if constexpr (std::same_as<T, std::string> || std::integral<T>) {
                return value;
            } else if constexpr (IsDefinitionID<T>::value) {
                if (path != "/id") {
                    if constexpr (std::same_as<T, UnlockID>) {
                        entry.m_unlocks.push_back({value.m_value, path});
                    } else {
                        entry.m_references.push_back({{kind_of<T>(), value.m_value}, path});
                    }
                }
                return value.m_value;
            } else if constexpr (IsVector<T>::value) {
                auto result = Json::array();
                for (std::size_t i = 0; i < value.size(); ++i)
                    result.push_back(project(value[i], entry, path + "/" + std::to_string(i)));
                return result;
            } else {
                return Projection<T>::read(value, entry, path);
            }
        }

        class Fields {
        public:
            Fields(CatalogEntry& entry, const std::string& path) : m_entry(entry), m_path(path) {}

            template <typename T>
            void add(const char* name, const T& value) {
                m_data[name] = project(value, m_entry, m_path + "/" + name);
            }

            Json take() {
                return std::move(m_data);
            }

        private:
            CatalogEntry&      m_entry;
            const std::string& m_path;
            Json               m_data = Json::object();
        };

        template <typename T, std::size_t N>
        Json enum_name(T value, const std::array<const char*, N>& names) {
            const auto index = static_cast<std::size_t>(value);
            return index < names.size() ? Json(names[index]) : Json("unknown");
        }

#define PROJECT_ENUM(Type, ...)                                           \
    template <>                                                           \
    struct Projection<Type> {                                             \
        static Json read(Type value, CatalogEntry&, const std::string&) { \
            return enum_name(value, std::array{__VA_ARGS__});             \
        }                                                                 \
    }

        PROJECT_ENUM(Stat_type, "strength", "agility", "vitality", "intelligence", "max_hp", "attack", "defense", "speed");
        PROJECT_ENUM(Modifier_operation, "flat_add", "additive_percent", "scale", "base_override");
        PROJECT_ENUM(Damage_kind, "physical", "fire", "ice", "poison");
        PROJECT_ENUM(Target_relation, "self", "ally", "enemy", "any");
        PROJECT_ENUM(Target_shape, "unit", "tile", "radius", "line");
        PROJECT_ENUM(Status_stack_policy, "replace", "refresh_duration", "stack_and_refresh");
        PROJECT_ENUM(Equipment_slot, "main_hand", "off_hand", "helmet", "chest", "gloves", "pants", "boots", "ear_ring_1", "ear_ring_2", "accessory_1", "accessory_2");
        PROJECT_ENUM(Reward_roll_policy, "weighted_with_replacement", "all_entries");
#undef PROJECT_ENUM

        template <>
        struct Projection<BasisPoints> {
            static Json read(BasisPoints value, CatalogEntry&, const std::string&) {
                return value.m_value;
            }
        };
        template <>
        struct Projection<FloorNumber> {
            static Json read(FloorNumber value, CatalogEntry&, const std::string&) {
                return value.m_value;
            }
        };
        template <>
        struct Projection<std::chrono::seconds> {
            static Json read(std::chrono::seconds value, CatalogEntry&, const std::string&) {
                return value.count();
            }
        };
        template <>
        struct Projection<StatBlock> {
            static Json read(const StatBlock& value, CatalogEntry&, const std::string&) {
                auto data = Json::object();
                for (std::size_t i = 0; i < stat_names.size(); ++i)
                    data[stat_names[i]] = value.m_values[i];
                return data;
            }
        };
        template <>
        struct Projection<ResourceBundle> {
            static Json read(const ResourceBundle& value, CatalogEntry& entry, const std::string& path) {
                auto data = Json::object();
                for (const auto& [id, amount] : value.m_amounts) {
                    data[id.m_value] = amount;
                    entry.m_references.push_back({{Catalog_kind::resources, id.m_value}, path + "/" + id.m_value, true});
                }
                return data;
            }
        };

// Each field list deliberately mirrors its C++ type, rather than the authored
// JSON. Keep this projection alongside additions to definitions.h.
#define PROJECT_STRUCT(Type, ...)                                                           \
    template <>                                                                             \
    struct Projection<Type> {                                                               \
        static Json read(const Type& value, CatalogEntry& entry, const std::string& path) { \
            Fields fields(entry, path);                                                     \
            __VA_ARGS__                                                                     \
            return fields.take();                                                           \
        }                                                                                   \
    }
#define FIELD(Name) fields.add(#Name, value.m_##Name)

        PROJECT_STRUCT(Modifier_source, FIELD(kind); FIELD(stable_key););
        PROJECT_STRUCT(StatModifier, FIELD(stat); FIELD(operation); FIELD(value); FIELD(priority); FIELD(source););
        PROJECT_STRUCT(DamageEffect, fields.add("kind", std::string("damage")); fields.add("damage_kind", value.m_kind); FIELD(base_amount); FIELD(scaling_stat); FIELD(scaling););
        PROJECT_STRUCT(HealEffect, fields.add("kind", std::string("heal")); FIELD(base_amount););
        PROJECT_STRUCT(ApplyStatusEffect, fields.add("kind", std::string("apply_status")); FIELD(status); FIELD(turns); FIELD(chance););
        PROJECT_STRUCT(MoveEffect, fields.add("kind", std::string("move")); FIELD(distance); FIELD(push););

        template <>
        struct Projection<EffectDefinition> {
            static Json read(const EffectDefinition& value, CatalogEntry& entry, const std::string& path) {
                return std::visit([&](const auto& effect) {
                    return project(effect, entry, path);
                },
                                  value);
            }
        };

        PROJECT_STRUCT(TargetingRule, FIELD(relation); FIELD(shape); FIELD(minimum_range); FIELD(maximum_range); FIELD(radius); FIELD(requires_line_of_sight););
        PROJECT_STRUCT(GridTile, FIELD(walkable); FIELD(movement_cost); FIELD(elevation); FIELD(blocks_sight););
        PROJECT_STRUCT(GridPosition, FIELD(x); FIELD(y););
        PROJECT_STRUCT(CharacterClassDefinition, FIELD(id); FIELD(name); FIELD(base_stats); FIELD(starting_abilities); FIELD(allowed_traits); FIELD(level_thresholds););
        PROJECT_STRUCT(TraitDefinition, FIELD(id); FIELD(name); FIELD(modifiers); FIELD(excludes););
        PROJECT_STRUCT(AbilityDefinition, FIELD(id); FIELD(name); FIELD(action_cost); FIELD(cooldown_turns); FIELD(targeting); FIELD(effects););
        PROJECT_STRUCT(StatusDefinition, FIELD(id); FIELD(name); FIELD(maximum_stacks); FIELD(stack_policy); FIELD(modifiers); FIELD(turn_start_effects););
        PROJECT_STRUCT(ItemDefinition, FIELD(id); FIELD(name); FIELD(slot); FIELD(maximum_level); FIELD(modifiers); FIELD(upgrade_costs););
        PROJECT_STRUCT(RecipeDefinition, FIELD(id); FIELD(cost); FIELD(outputs); FIELD(required_unlocks););
        PROJECT_STRUCT(BannerDefinition, FIELD(id); FIELD(cost_per_summon); FIELD(allowed_classes); FIELD(required_unlocks););
        PROJECT_STRUCT(ResourceDefinition, FIELD(id); FIELD(name););
        PROJECT_STRUCT(EnemyDefinition, FIELD(id); FIELD(class_id); FIELD(stats); FIELD(abilities););
        PROJECT_STRUCT(BattlefieldTemplate, FIELD(id); FIELD(width); FIELD(height); FIELD(tiles); FIELD(ally_spawns); FIELD(enemy_spawns););
        PROJECT_STRUCT(ObjectiveDefinition, FIELD(id); FIELD(kind); FIELD(target_count););
        PROJECT_STRUCT(EncounterDefinition, FIELD(id); FIELD(battlefield); FIELD(enemies); FIELD(objectives); FIELD(rewards););
        PROJECT_STRUCT(RegionDefinition, FIELD(id); FIELD(first_floor); FIELD(last_floor); FIELD(battlefields); FIELD(enemies););
        PROJECT_STRUCT(CampBuildingDefinition, FIELD(id); FIELD(maximum_level); FIELD(construction_cost); FIELD(upgrade_costs); FIELD(modifiers); FIELD(required_unlocks););
        PROJECT_STRUCT(RewardEntry, FIELD(resource); FIELD(minimum); FIELD(maximum); FIELD(weight););
        PROJECT_STRUCT(ItemRewardEntry, FIELD(definition); FIELD(weight););
        PROJECT_STRUCT(RewardTableDefinition, FIELD(id); FIELD(entries); FIELD(rolls); FIELD(policy); FIELD(experience_per_participant); FIELD(item_entries); FIELD(item_rolls););
        PROJECT_STRUCT(IdleActivityDefinition, FIELD(id); FIELD(rewards); FIELD(interval); FIELD(offline_cap););
#undef FIELD
#undef PROJECT_STRUCT

        template <typename T>
        void append(CatalogSnapshot& snapshot, std::span<const T> definitions) {
            for (const auto& definition : definitions) {
                CatalogEntry entry;
                entry.m_key = {kind_of<decltype(definition.m_id)>(), definition.m_id.m_value};
                if constexpr (requires { definition.m_name; })
                    entry.m_name = definition.m_name;
                entry.m_fields = project(definition, entry, "");
                snapshot.m_index.emplace(entry.m_key, snapshot.m_entries.size());
                snapshot.m_entries.push_back(std::move(entry));
            }
        }

    } // namespace

    const CatalogEntry* CatalogSnapshot::find(const CatalogKey& key) const {
        const auto it = m_index.find(key);
        return it == m_index.end() ? nullptr : &m_entries[it->second];
    }

    std::span<const CatalogCategory> catalog_categories() noexcept {
        return categories;
    }

    CatalogSnapshot inspect_catalog(const ContentCatalog& catalog) {
        CatalogSnapshot snapshot;
        snapshot.m_content_version = catalog.content_version();
        append(snapshot, catalog.classes());
        append(snapshot, catalog.traits());
        append(snapshot, catalog.abilities());
        append(snapshot, catalog.statuses());
        append(snapshot, catalog.items());
        append(snapshot, catalog.recipes());
        append(snapshot, catalog.banners());
        append(snapshot, catalog.resources());
        append(snapshot, catalog.enemies());
        append(snapshot, catalog.battlefields());
        append(snapshot, catalog.objectives());
        append(snapshot, catalog.encounters());
        append(snapshot, catalog.regions());
        append(snapshot, catalog.buildings());
        append(snapshot, catalog.rewards());
        append(snapshot, catalog.idle_activities());
        for (const auto& entry : snapshot.m_entries) {
            for (const auto& reference : entry.m_references)
                snapshot.m_used_by[reference.m_target].push_back({entry.m_key, reference.m_path});
        }
        return snapshot;
    }

} // namespace debug