#ifndef PROJECTSKYLAKE_IDS_H
#define PROJECTSKYLAKE_IDS_H

#include <string>
#include <cstdint>

template <class Tag>
struct EntityID {
    std::int64_t m_value{};
    auto         operator<=>(const EntityID&) const = default;
};

template <class Tag>
struct DefinitionID {
    std::string m_value;
    auto        operator<=>(const DefinitionID&) const = default;
};

struct CharacterTag;
using CharacterID = EntityID<CharacterTag>;

struct ItemTag;
using ItemID = EntityID<ItemTag>;

struct TeamTag;
using TeamID = EntityID<TeamTag>;

struct BattleTag;
using BattleID = EntityID<BattleTag>;

struct CombatantTag;
using CombatantID = EntityID<CombatantTag>;

struct BuildingTag;
using BuildingID = EntityID<BuildingTag>;

struct IdleAssignmentTag;
using IdleAssignmentID = EntityID<IdleAssignmentTag>;

struct ReceiptTag;
using ReceiptID = EntityID<ReceiptTag>;

struct SkillNodeTag;
using SkillNodeID = EntityID<SkillNodeTag>;

struct ClassTag;
using ClassID = DefinitionID<ClassTag>;

struct AbilityTag;
using AbilityID = DefinitionID<AbilityTag>;

struct TraitTag;
using TraitID = DefinitionID<TraitTag>;

struct StatusTag;
using StatusID = DefinitionID<StatusTag>;

struct ItemDefinitionTag;
using ItemDefinitionID = DefinitionID<ItemDefinitionTag>;

struct RecipeTag;
using RecipeID = DefinitionID<RecipeTag>;

struct BannerTag;
using BannerID = DefinitionID<BannerTag>;

struct RegionTag;
using RegionID = DefinitionID<RegionTag>;

struct EnemyTag;
using EnemyID = DefinitionID<EnemyTag>;

struct BattlefieldTag;
using BattlefieldID = DefinitionID<BattlefieldTag>;

struct EncounterTag;
using EncounterID = DefinitionID<EncounterTag>;

struct ObjectiveTag;
using ObjectiveID = DefinitionID<ObjectiveTag>;

struct BuildingDefinitionTag;
using BuildingDefinitionID = DefinitionID<BuildingDefinitionTag>;

struct ResourceTag;
using ResourceID = DefinitionID<ResourceTag>;

struct RewardTableTag;
using RewardTableID = DefinitionID<RewardTableTag>;

struct IdleActivityTag;
using IdleActivityID = DefinitionID<IdleActivityTag>;

struct UnlockTag;
using UnlockID = DefinitionID<UnlockTag>;

#endif //PROJECTSKYLAKE_IDS_H
