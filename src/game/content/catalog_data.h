#ifndef PROJECTSKYLAKE_CATALOG_DATA_H
#define PROJECTSKYLAKE_CATALOG_DATA_H

#include "definitions.h"

#include <vector>

struct CatalogData {
    std::vector<CharacterClassDefinition> m_classes;
    std::vector<TraitDefinition>          m_traits;
    std::vector<AbilityDefinition>        m_abilities;
    std::vector<StatusDefinition>         m_statuses;
    std::vector<ItemDefinition>           m_equipment;
    std::vector<RecipeDefinition>         m_recipes;
    std::vector<BannerDefinition>         m_banners;
    std::vector<ResourceDefinition>       m_resources;
    std::vector<EnemyDefinition>          m_enemies;
    std::vector<BattlefieldTemplate>      m_battlefields;
    std::vector<ObjectiveDefinition>      m_objectives;
    std::vector<EncounterDefinition>      m_encounters;
    std::vector<RegionDefinition>         m_regions;
    std::vector<CampBuildingDefinition>   m_buildings;
    std::vector<RewardTableDefinition>    m_reward_tables;
    std::vector<IdleActivityDefinition>   m_idle_activities;
};

#endif //PROJECTSKYLAKE_CATALOG_DATA_H
