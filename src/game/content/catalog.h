#ifndef PROJECTSKYLAKE_CATALOG_H
#define PROJECTSKYLAKE_CATALOG_H

#include "catalog_data.h"
#include "definition_registry.h"
#include "definitions.h"
#include "game/core/ids.h"
#include "game/core/results.h"

#include <unordered_map>

class ContentCatalog {
public:
    // A valid empty catalog is used while GameRuntime is being constructed.
    ContentCatalog() = default;
    [[nodiscard]] const std::string&                        content_version() const noexcept;
    [[nodiscard]] std::span<const CharacterClassDefinition> classes() const noexcept;
    [[nodiscard]] std::span<const TraitDefinition>          traits() const noexcept;
    [[nodiscard]] std::span<const AbilityDefinition>        abilities() const noexcept;
    [[nodiscard]] std::span<const StatusDefinition>         statuses() const noexcept;
    [[nodiscard]] std::span<const ItemDefinition>           items() const noexcept;
    [[nodiscard]] std::span<const RecipeDefinition>         recipes() const noexcept;
    [[nodiscard]] std::span<const BannerDefinition>         banners() const noexcept;
    [[nodiscard]] std::span<const ResourceDefinition>       resources() const noexcept;
    [[nodiscard]] std::span<const EnemyDefinition>          enemies() const noexcept;
    [[nodiscard]] std::span<const BattlefieldTemplate>      battlefields() const noexcept;
    [[nodiscard]] std::span<const ObjectiveDefinition>      objectives() const noexcept;
    [[nodiscard]] std::span<const EncounterDefinition>      encounters() const noexcept;
    [[nodiscard]] std::span<const RegionDefinition>         regions() const noexcept;
    [[nodiscard]] std::span<const CampBuildingDefinition>   buildings() const noexcept;
    [[nodiscard]] std::span<const RewardTableDefinition>    rewards() const noexcept;
    [[nodiscard]] std::span<const IdleActivityDefinition>   idle_activities() const noexcept;
    [[nodiscard]] const CharacterClassDefinition*           find(const ClassID& id) const;
    [[nodiscard]] const TraitDefinition*                    find(const TraitID& id) const;
    [[nodiscard]] const AbilityDefinition*                  find(const AbilityID& id) const;
    [[nodiscard]] const StatusDefinition*                   find(const StatusID& id) const;
    [[nodiscard]] const ItemDefinition*                     find(const ItemDefinitionID& id) const;
    [[nodiscard]] const RecipeDefinition*                   find(const RecipeID& id) const;
    [[nodiscard]] const BannerDefinition*                   find(const BannerID& id) const;
    [[nodiscard]] const ResourceDefinition*                 find(const ResourceID& id) const;
    [[nodiscard]] const EnemyDefinition*                    find(const EnemyID& id) const;
    [[nodiscard]] const BattlefieldTemplate*                find(const BattlefieldID& id) const;
    [[nodiscard]] const ObjectiveDefinition*                find(const ObjectiveID& id) const;
    [[nodiscard]] const EncounterDefinition*                find(const EncounterID& id) const;
    [[nodiscard]] const RegionDefinition*                   find(const RegionID& id) const;
    [[nodiscard]] const CampBuildingDefinition*             find(const BuildingDefinitionID& id) const;
    [[nodiscard]] const RewardTableDefinition*              find(const RewardTableID& id) const;
    [[nodiscard]] const IdleActivityDefinition*             find(const IdleActivityID& id) const;
    [[nodiscard]] static Result<ContentCatalog>             build(CatalogData data);
    [[nodiscard]] Result<void>                              validate() const;

private:
    std::string                                  m_content_version;
    DefinitionRegistry<CharacterClassDefinition> m_classes;
    DefinitionRegistry<TraitDefinition>          m_traits;
    DefinitionRegistry<AbilityDefinition>        m_abilities;
    DefinitionRegistry<StatusDefinition>         m_statuses;
    DefinitionRegistry<ItemDefinition>           m_items;
    DefinitionRegistry<RecipeDefinition>         m_recipes;
    DefinitionRegistry<BannerDefinition>         m_banners;
    DefinitionRegistry<ResourceDefinition>       m_resources;
    DefinitionRegistry<EnemyDefinition>          m_enemies;
    DefinitionRegistry<BattlefieldTemplate>      m_battlefields;
    DefinitionRegistry<ObjectiveDefinition>      m_objectives;
    DefinitionRegistry<EncounterDefinition>      m_encounters;
    DefinitionRegistry<RegionDefinition>         m_regions;
    DefinitionRegistry<CampBuildingDefinition>   m_buildings;
    DefinitionRegistry<RewardTableDefinition>    m_rewards;
    DefinitionRegistry<IdleActivityDefinition>   m_idle_activities;
};

#endif //PROJECTSKYLAKE_CATALOG_H
