#ifndef PROJECTSKYLAKE_CATALOG_H
#define PROJECTSKYLAKE_CATALOG_H

#include "definitions.h"
#include "game/core/ids.h"
#include "game/core/results.h"

#include <unordered_map>


class ContentCatalog {
public:
    ContentCatalog() = default;
    ~ContentCatalog() = default;
    Result<void>                    validate() const;
    const CharacterClassDefinition* find(const ClassID&) const;
    const TraitDefinition*          find(const TraitID&) const;
    const AbilityDefinition*        find(const AbilityID&) const;
    const StatusDefinition*         find(const StatusID&) const;
    const ItemDefinition*           find(const ItemDefinitionID&) const;
    const RecipeDefinition*         find(const RecipeID&) const;
    const BannerDefinition*         find(const BannerID&) const;
    const ResourceDefinition*       find(const ResourceID&) const;
    const EnemyDefinition*          find(const EnemyID&) const;
    const BattlefieldTemplate*      find(const BattlefieldID&) const;
    const ObjectiveDefinition*      find(const ObjectiveID&) const;
    const EncounterDefinition*      find(const EncounterID&) const;
    const RegionDefinition*         find(const RegionID&) const;
    const CampBuildingDefinition*   find(const BuildingDefinitionID&) const;
    const RewardTableDefinition*    find(const RewardTableID&) const;
    const IdleActivityDefinition*   find(const IdleActivityID&) const;

private:
    std::unordered_map<ClassID, CharacterClassDefinition, IDHash>      m_classes;
    std::unordered_map<TraitID, TraitDefinition, IDHash>               m_traits;
    std::unordered_map<AbilityID, AbilityDefinition, IDHash>           m_abilities;
    std::unordered_map<StatusID, StatusDefinition, IDHash>             m_statuses;
    std::unordered_map<ItemDefinitionID, ItemDefinition, IDHash>       m_equipment;
    std::unordered_map<RecipeID, RecipeDefinition, IDHash>             m_recipes;
    std::unordered_map<BannerID, BannerDefinition, IDHash>             m_banners;
    std::unordered_map<ResourceID, ResourceDefinition, IDHash>         m_resources;
    std::unordered_map<EnemyID, EnemyDefinition, IDHash>               m_enemies;
    std::unordered_map<BattlefieldID, BattlefieldTemplate, IDHash>     m_battles;
    std::unordered_map<ObjectiveID, ObjectiveDefinition, IDHash>       m_objectives;
    std::unordered_map<EncounterID, EncounterDefinition, IDHash>       m_encounters;
    std::unordered_map<RegionID, RegionDefinition, IDHash>             m_regions;
    std::unordered_map<BuildingID, CampBuildingDefinition, IDHash>     m_buildings;
    std::unordered_map<RewardTableID, RewardTableDefinition, IDHash>   m_rewards;
    std::unordered_map<IdleActivityID, IdleActivityDefinition, IDHash> m_idle_activities;
};

#endif //PROJECTSKYLAKE_CATALOG_H
