#ifndef PROJECTSKYLAKE_DEFINITIONS_H
#define PROJECTSKYLAKE_DEFINITIONS_H

#include "game/combat/grid.h"
#include "game/core/ids.h"
#include "game/core/types.h"
#include "game/economy/resources.h"
#include "game/items/equipment.h"
#include "game/character/stats.h"

#include <chrono>
#include <variant>

struct CharacterClassDefinition {
    ClassID                   m_id;
    std::string               m_name;
    StatBlock                 m_base_stats;
    std::vector<AbilityID>    m_starting_abilities;
    std::vector<TraitID>      m_allowed_traits;
    std::vector<std::int64_t> m_level_thresholds;
};

struct TraitDefinition {
    TraitID                   m_id;
    std::string               m_name;
    std::vector<StatModifier> m_modifiers;
    std::vector<TraitID>      m_excludes;
};

enum class Damage_kind {
    physical,
    fire,
    ice,
    poison
};

struct DamageEffect {
    Damage_kind  m_kind;
    std::int64_t m_base_amount{};
    Stat_type    m_scaling_stat{Stat_type::attack};
    BasisPoints  m_scaling{};
};

struct HealEffect {
    std::int64_t m_base_amount{};
};

struct ApplyStatusEffect {
    StatusID     m_status;
    std::int32_t m_turns{1};
    BasisPoints  m_chance{10'000};
};

struct MoveEffect {
    std::int32_t m_distance{1};
    bool         m_push{true};
};

using EffectDefinition = std::variant<DamageEffect, HealEffect, ApplyStatusEffect, MoveEffect>;

enum class Target_relation {
    self,
    ally,
    enemy,
    any
};

enum class Target_shape {
    unit,
    tile,
    radius,
    line
};

struct TargetingRule {
    Target_relation m_relation;
    Target_shape    m_shape;
    std::int32_t    m_minimum_range{};
    std::int32_t    m_maximum_range{1};
    std::int32_t    m_radius{};
    bool            m_requires_line_of_sight{true};
};

struct AbilityDefinition {
    AbilityID                     m_id;
    std::string                   m_name;
    std::int32_t                  m_action_cost{1};
    std::int32_t                  m_cooldown_turns{};
    TargetingRule                 m_targeting;
    std::vector<EffectDefinition> m_effects;
};

enum class Status_stack_policy {
    replace,
    refresh_duration,
    stack_and_refresh
};

struct StatusDefinition {
    StatusID                      m_id;
    std::string                   m_name;
    std::int32_t                  m_maximum_stacks{1};
    Status_stack_policy           m_stack_policy{Status_stack_policy::refresh_duration};
    std::vector<StatModifier>     m_modifiers;
    std::vector<EffectDefinition> m_turn_start_effects;
};

struct ItemDefinition {
    ItemDefinitionID          m_id;
    std::string               m_name;
    Equipment_slot            m_slot;
    std::vector<StatModifier> m_modifiers;
    std::int32_t              m_maximum_level{1};
    std::vector<ResourceCost> m_upgrade_costs;
};

struct RecipeDefinition {
    RecipeID                      m_id;
    ResourceCost                  m_cost;
    std::vector<ItemDefinitionID> m_outputs;
    std::vector<UnlockID>         m_required_unlocks;
};

struct BannerDefinition {
    BannerID              m_id;
    ResourceCost          m_cost_per_summon;
    std::vector<ClassID>  m_allowed_classes;
    std::vector<UnlockID> m_required_unlocks;
};

struct ResourceDefinition {
    ResourceID  m_id;
    std::string m_name;
};

struct EnemyDefinition {
    EnemyID                m_id;
    ClassID                m_class_id;
    StatBlock              m_stats;
    std::vector<AbilityID> m_abilities;
};

struct BattlefieldTemplate {
    BattlefieldID             m_id;
    std::int32_t              m_width{};
    std::int32_t              m_height{};
    std::vector<GridTile>     m_tiles;
    std::vector<GridPosition> m_ally_spawns;
    std::vector<GridPosition> m_enemy_spawns;
};

struct ObjectiveDefinition {
    ObjectiveID  m_id;
    std::string  m_kind;
    std::int32_t m_target_count{};
};

struct EncounterDefinition {
    EncounterID              m_id;
    BattlefieldID            m_battlefield;
    std::vector<EnemyID>     m_enemies;
    std::vector<ObjectiveID> m_objectives;
    RewardTableID            m_rewards;
};

struct RegionDefinition {
    RegionID                   m_id;
    FloorNumber                m_first_floor;
    FloorNumber                m_last_floor;
    std::vector<BattlefieldID> m_battlefields;
    std::vector<EnemyID>       m_enemies;
};

struct CampBuildingDefinition {
    BuildingDefinitionID      m_id;
    std::int32_t              m_maximum_level{1};
    ResourceCost              m_construction_cost;
    std::vector<ResourceCost> m_upgrade_costs;
    std::vector<StatModifier> m_modifiers;
    std::vector<UnlockID>     m_required_unlocks;
};

struct RewardEntry {
    ResourceID    m_resource;
    std::int64_t  m_minimum{};
    std::int64_t  m_maximum{};
    std::uint64_t m_weight{1};
};

struct ItemRewardEntry {
    ItemDefinitionID m_definition;
    std::uint64_t    m_weight{1};
};

enum class Reward_roll_policy {
    weighted_with_replacement,
    all_entries
};

struct RewardTableDefinition {
    RewardTableID                m_id;
    std::vector<RewardEntry>     m_entries;
    std::int32_t                 m_rolls{1};
    Reward_roll_policy           m_policy{Reward_roll_policy::weighted_with_replacement};
    std::int64_t                 m_experience_per_participant{};
    std::vector<ItemRewardEntry> m_item_entries;
    std::int32_t                 m_item_rolls{};
};

struct IdleActivityDefinition {
    IdleActivityID       m_id;
    RewardTableID        m_rewards;
    std::chrono::seconds m_interval{60};
    std::chrono::seconds m_offline_cap{28'800};
};

#endif //PROJECTSKYLAKE_DEFINITIONS_H
