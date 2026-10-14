#ifndef PROJECTSKYLAKE_ID_H
#define PROJECTSKYLAKE_ID_H

#include <cstdint>
#include <string>

struct CharacterTag;
struct ItemTag;
struct TeamTag;
struct BattleTag;
struct CombatantTag;
struct BuildingTag;
struct IdleAssignmentTag;
struct ReceiptTag;
struct SkillNodeTag;
struct ClassTag;
struct AbilityTag;
struct TraitTag;
struct StatusTag;
struct ItemDefinitionTag;
struct RecipeTag;
struct BannerTag;
struct RegionTag;
struct EnemyTag;
struct BattlefieldTag;
struct EncounterTag;
struct ObjectiveTag;
struct BuildingDefinitionTag;
struct ResourceTag;
struct RewardTableTag;
struct IdleActivityTag;
struct UnlockTag;

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

using CharacterID = EntityID<CharacterTag>;
using ItemID = EntityID<ItemTag>;
using TeamID = EntityID<TeamTag>;
using BattleID = EntityID<BattleTag>;
using CombatantID = EntityID<CombatantTag>;
using BuildingID = EntityID<BuildingTag>;
using IdleAssignmentID = EntityID<IdleAssignmentTag>;
using ReceiptID = EntityID<ReceiptTag>;
using SkillNodeID = EntityID<SkillNodeTag>;
using ClassID = DefinitionID<ClassTag>;
using AbilityID = DefinitionID<AbilityTag>;
using TraitID = DefinitionID<TraitTag>;
using StatusID = DefinitionID<StatusTag>;
using ItemDefinitionID = DefinitionID<ItemDefinitionTag>;
using RecipeID = DefinitionID<RecipeTag>;
using BannerID = DefinitionID<BannerTag>;
using RegionID = DefinitionID<RegionTag>;
using EnemyID = DefinitionID<EnemyTag>;
using BattlefieldID = DefinitionID<BattlefieldTag>;
using EncounterID = DefinitionID<EncounterTag>;
using ObjectiveID = DefinitionID<ObjectiveTag>;
using BuildingDefinitionID = DefinitionID<BuildingDefinitionTag>;
using ResourceID = DefinitionID<ResourceTag>;
using RewardTableID = DefinitionID<RewardTableTag>;
using IdleActivityID = DefinitionID<IdleActivityTag>;
using UnlockID = DefinitionID<UnlockTag>;


#endif //PROJECTSKYLAKE_ID_H
