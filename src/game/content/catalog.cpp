#include "catalog.h"
#include "validation.h"
#include <utility>

namespace {

    template <typename Definition>
    Result<void> build_registry(DefinitionRegistry<Definition>& destination, std::vector<Definition> definitions, const std::string_view kind) {
        auto result = DefinitionRegistry<Definition>::build(std::move(definitions), kind);
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        destination = std::get<DefinitionRegistry<Definition>>(std::move(result));
        return std::monostate{};
    }

}

const std::string& ContentCatalog::content_version() const noexcept {
    return m_content_version;
}

std::span<const CharacterClassDefinition> ContentCatalog::classes() const noexcept {
    return m_classes.all();
}

std::span<const TraitDefinition> ContentCatalog::traits() const noexcept {
    return m_traits.all();
}

std::span<const AbilityDefinition> ContentCatalog::abilities() const noexcept {
    return m_abilities.all();
}

std::span<const StatusDefinition> ContentCatalog::statuses() const noexcept {
    return m_statuses.all();
}

std::span<const ItemDefinition> ContentCatalog::items() const noexcept {
    return m_items.all();
}

std::span<const RecipeDefinition> ContentCatalog::recipes() const noexcept {
    return m_recipes.all();
}

std::span<const BannerDefinition> ContentCatalog::banners() const noexcept {
    return m_banners.all();
}

std::span<const ResourceDefinition> ContentCatalog::resources() const noexcept {
    return m_resources.all();
}

std::span<const EnemyDefinition> ContentCatalog::enemies() const noexcept {
    return m_enemies.all();
}

std::span<const BattlefieldTemplate> ContentCatalog::battlefields() const noexcept {
    return m_battlefields.all();
}

std::span<const ObjectiveDefinition> ContentCatalog::objectives() const noexcept {
    return m_objectives.all();
}

std::span<const EncounterDefinition> ContentCatalog::encounters() const noexcept {
    return m_encounters.all();
}

std::span<const RegionDefinition> ContentCatalog::regions() const noexcept {
    return m_regions.all();
}

std::span<const CampBuildingDefinition> ContentCatalog::buildings() const noexcept {
    return m_buildings.all();
}

std::span<const RewardTableDefinition> ContentCatalog::rewards() const noexcept {
    return m_rewards.all();
}

std::span<const IdleActivityDefinition> ContentCatalog::idle_activities() const noexcept {
    return m_idle_activities.all();
}

const CharacterClassDefinition* ContentCatalog::find(const ClassID& id) const {
    return m_classes.find(id);
}

const TraitDefinition* ContentCatalog::find(const TraitID& id) const {
    return m_traits.find(id);
}

const AbilityDefinition* ContentCatalog::find(const AbilityID& id) const {
    return m_abilities.find(id);
}

const StatusDefinition* ContentCatalog::find(const StatusID& id) const {
    return m_statuses.find(id);
}

const ItemDefinition* ContentCatalog::find(const ItemDefinitionID& id) const {
    return m_items.find(id);
}

const RecipeDefinition* ContentCatalog::find(const RecipeID& id) const {
    return m_recipes.find(id);
}

const BannerDefinition* ContentCatalog::find(const BannerID& id) const {
    return m_banners.find(id);
}

const ResourceDefinition* ContentCatalog::find(const ResourceID& id) const {
    return m_resources.find(id);
}

const EnemyDefinition* ContentCatalog::find(const EnemyID& id) const {
    return m_enemies.find(id);
}

const BattlefieldTemplate* ContentCatalog::find(const BattlefieldID& id) const {
    return m_battlefields.find(id);
}

const ObjectiveDefinition* ContentCatalog::find(const ObjectiveID& id) const {
    return m_objectives.find(id);
}

const EncounterDefinition* ContentCatalog::find(const EncounterID& id) const {
    return m_encounters.find(id);
}

const RegionDefinition* ContentCatalog::find(const RegionID& id) const {
    return m_regions.find(id);
}

const CampBuildingDefinition* ContentCatalog::find(const BuildingDefinitionID& id) const {
    return m_buildings.find(id);
}

const RewardTableDefinition* ContentCatalog::find(const RewardTableID& id) const {
    return m_rewards.find(id);
}

const IdleActivityDefinition* ContentCatalog::find(const IdleActivityID& id) const {
    return m_idle_activities.find(id);
}

Result<ContentCatalog> ContentCatalog::build(CatalogData data) {
    if (data.m_content_version.empty()) {
        return Error{
            .m_code = Error_code::invalid_content,
            .m_message = "catalog: content_version must not be empty"
        };
    }
    ContentCatalog candidate;
    candidate.m_content_version = std::move(data.m_content_version);
    if (auto result = build_registry(candidate.m_classes, std::move(data.m_classes), "classes"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_traits, std::move(data.m_traits), "traits"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_abilities, std::move(data.m_abilities), "abilities"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_statuses, std::move(data.m_statuses), "statuses"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_items, std::move(data.m_items), "items"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_recipes, std::move(data.m_recipes), "recipes"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_banners, std::move(data.m_banners), "banners"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_resources, std::move(data.m_resources), "resources"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_enemies, std::move(data.m_enemies), "enemies"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_battlefields, std::move(data.m_battlefields), "battlefields"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_objectives, std::move(data.m_objectives), "objectives"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_encounters, std::move(data.m_encounters), "encounters"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_regions, std::move(data.m_regions), "regions"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_buildings, std::move(data.m_buildings), "buildings"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_rewards, std::move(data.m_rewards), "rewards"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    if (auto result = build_registry(candidate.m_idle_activities, std::move(data.m_idle_activities), "idle_activities"); const auto* error = std::get_if<Error>(&result)) {
        return *error;
    }
    //valid if the ingested content is appropriate
    if (auto validation = candidate.validate(); const auto* error = std::get_if<Error>(&validation)) {
        return *error;
    }
    return candidate;
}

Result<void> ContentCatalog::validate() const {
    return validate_catalog(*this);
}
