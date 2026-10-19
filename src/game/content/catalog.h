#ifndef PROJECTSKYLAKE_CATALOG_H
#define PROJECTSKYLAKE_CATALOG_H

#include "definitions.h"
#include "game/core/ids.h"
#include "game/core/results.h"

#include <string>
#include <vector>

struct ContentVersion {
    std::string m_value;
};

struct CatalogData {
    ContentVersion                        m_version;
    std::vector<CharacterClassDefinition> m_classes;
    std::vector<TraitDefinition>          m_traits;
    std::vector<AbilityDefinition>        m_abilities;
    std::vector<StatusDefinition>         m_statuses;
    std::vector<EquipmentDefinition>      m_equipment;
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

class ContentCatalog {
public:
    explicit ContentCatalog(CatalogData);
    ~ContentCatalog();
    Result<void>                    validate() const;
    ContentVersion                  version() const;
    const CharacterClassDefinition* find(const ClassID&) const;
    const TraitDefinition*          find(const TraitID&) const;
    const AbilityDefinition*        find(const AbilityID&) const;
    const StatusDefinition*         find(const StatusID&) const;
    const EquipmentDefinition*      find(const ItemDefinitionID&) const;
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
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

class IContentSource {
public:
    virtual ~IContentSource() = default;
    virtual Result<CatalogData> load() = 0;
};

#endif //PROJECTSKYLAKE_CATALOG_H
