#include "catalog.h"

#include "validation.h"

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
    {
        auto result = DefinitionRegistry<CharacterClassDefinition>::build(std::move(data.m_classes), "classes");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_classes = std::get<DefinitionRegistry<CharacterClassDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<TraitDefinition>::build(std::move(data.m_traits), "traits");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_traits = std::get<DefinitionRegistry<TraitDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<AbilityDefinition>::build(std::move(data.m_abilities), "abilities");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_abilities = std::get<DefinitionRegistry<AbilityDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<StatusDefinition>::build(std::move(data.m_statuses), "statuses");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_statuses = std::get<DefinitionRegistry<StatusDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<ItemDefinition>::build(std::move(data.m_items), "items");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_items = std::get<DefinitionRegistry<ItemDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<RecipeDefinition>::build(std::move(data.m_recipes), "recipes");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_recipes = std::get<DefinitionRegistry<RecipeDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<BannerDefinition>::build(std::move(data.m_banners), "banners");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_banners = std::get<DefinitionRegistry<BannerDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<ResourceDefinition>::build(std::move(data.m_resources), "resources");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_resources = std::get<DefinitionRegistry<ResourceDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<EnemyDefinition>::build(std::move(data.m_enemies), "enemies");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_enemies = std::get<DefinitionRegistry<EnemyDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<BattlefieldTemplate>::build(std::move(data.m_battlefields), "battlefields");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_battlefields = std::get<DefinitionRegistry<BattlefieldTemplate>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<ObjectiveDefinition>::build(std::move(data.m_objectives), "objectives");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_objectives = std::get<DefinitionRegistry<ObjectiveDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<EncounterDefinition>::build(std::move(data.m_encounters), "encounters");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_encounters = std::get<DefinitionRegistry<EncounterDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<RegionDefinition>::build(std::move(data.m_regions), "regions");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_regions = std::get<DefinitionRegistry<RegionDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<CampBuildingDefinition>::build(std::move(data.m_buildings), "buildings");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_buildings = std::get<DefinitionRegistry<CampBuildingDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<RewardTableDefinition>::build(std::move(data.m_rewards), "rewards");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_rewards = std::get<DefinitionRegistry<RewardTableDefinition>>(std::move(result));
    }
    {
        auto result = DefinitionRegistry<IdleActivityDefinition>::build(std::move(data.m_idle_activities), "idle_activities");
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
        candidate.m_idle_activities = std::get<DefinitionRegistry<IdleActivityDefinition>>(std::move(result));
    }
    auto validation = candidate.validate();
    if (const auto* error = std::get_if<Error>(&validation)) {
        return *error;
    }
    return candidate;
}

Result<void> ContentCatalog::validate() const {
    Validator validator{*this};
    for (const auto& definition : classes()) {
        const auto path = location("classes", definition);
        validator.check(!definition.m_name.empty(), path + "/name: must not be empty");
        validator.stats(definition.m_base_stats, path + "/base_stats");
        validator.references(definition.m_starting_abilities, path + "/starting_abilities");
        validator.references(definition.m_allowed_traits, path + "/allowed_traits");
        std::int64_t previous = -1;
        for (const auto threshold : definition.m_level_thresholds) {
            validator.check(threshold >= 0 && threshold > previous, path + "/level_thresholds: must be nonnegative and strictly increasing");
            previous = threshold;
        }
    }
    for (const auto& definition : traits()) {
        const auto path = location("traits", definition);
        validator.check(!definition.m_name.empty(), path + "/name: must not be empty");
        validator.modifiers(definition.m_modifiers, path + "/modifiers");
        validator.references(definition.m_excludes, path + "/excludes");
        for (const auto& excluded : definition.m_excludes) {
            validator.check(excluded != definition.m_id, path + "/excludes: trait cannot exclude itself");
        }
    }
    for (const auto& definition : abilities()) {
        const auto path = location("abilities", definition);
        validator.check(!definition.m_name.empty(), path + "/name: must not be empty");
        validator.check(definition.m_action_cost >= 0 && definition.m_cooldown_turns >= 0, path + ": cost and cooldown must be nonnegative");
        const auto& targeting = definition.m_targeting;
        validator.check(enum_below(targeting.m_relation, 4) && enum_below(targeting.m_shape, 4), path + "/targeting: invalid relation or shape");
        validator.check(targeting.m_minimum_range >= 0 && targeting.m_maximum_range >= targeting.m_minimum_range && targeting.m_radius >= 0, path + "/targeting: invalid range or radius");
        validator.check(!definition.m_effects.empty(), path + "/effects: at least one effect is required");
        validator.effects(definition.m_effects, path + "/effects");
    }
    for (const auto& definition : statuses()) {
        const auto path = location("statuses", definition);
        validator.check(!definition.m_name.empty(), path + "/name: must not be empty");
        validator.check(definition.m_maximum_stacks > 0, path + "/maximum_stacks: must be positive");
        validator.check(enum_below(definition.m_stack_policy, 3), path + "/stack_policy: invalid policy");
        validator.modifiers(definition.m_modifiers, path + "/modifiers");
        validator.effects(definition.m_turn_start_effects, path + "/turn_start_effects");
    }
    for (const auto& definition : items()) {
        const auto path = location("items", definition);
        validator.check(!definition.m_name.empty(), path + "/name: must not be empty");
        validator.check(enum_below(definition.m_slot, static_cast<int>(Equipment_slot::max_equipment_slot_count)), path + "/slot: invalid slot");
        validator.check(definition.m_maximum_level > 0, path + "/maximum_level: must be positive");
        validator.modifiers(definition.m_modifiers, path + "/modifiers");
        validator.costs(definition.m_upgrade_costs, path + "/upgrade_costs");
    }
    for (const auto& definition : recipes()) {
        const auto path = location("recipes", definition);
        validator.cost(definition.m_cost, path + "/cost");
        validator.check(!definition.m_outputs.empty(), path + "/outputs: must not be empty");
        validator.references(definition.m_outputs, path + "/outputs");
        validator.unlocks(definition.m_required_unlocks, path + "/required_unlocks");
    }
    for (const auto& definition : banners()) {
        const auto path = location("banners", definition);
        validator.cost(definition.m_cost_per_summon, path + "/cost_per_summon");
        validator.check(!definition.m_allowed_classes.empty(), path + "/allowed_classes: must not be empty");
        validator.references(definition.m_allowed_classes, path + "/allowed_classes");
        validator.unlocks(definition.m_required_unlocks, path + "/required_unlocks");
    }
    for (const auto& definition : resources()) {
        validator.check(!definition.m_name.empty(), location("resources", definition) + "/name: must not be empty");
    }
    for (const auto& definition : enemies()) {
        const auto path = location("enemies", definition);
        validator.reference(definition.m_class_id, path + "/class_id");
        validator.stats(definition.m_stats, path + "/stats");
        validator.references(definition.m_abilities, path + "/abilities");
    }
    for (const auto& definition : battlefields()) {
        validator.battlefield(definition, location("battlefields", definition));
    }
    for (const auto& definition : objectives()) {
        const auto path = location("objectives", definition);
        validator.check(!definition.m_kind.empty(), path + "/kind: must not be empty");
        validator.check(definition.m_target_count > 0, path + "/target_count: must be positive");
    }
    for (const auto& definition : encounters()) {
        const auto path = location("encounters", definition);
        validator.reference(definition.m_battlefield, path + "/battlefield");
        validator.references(definition.m_enemies, path + "/enemies");
        validator.references(definition.m_objectives, path + "/objectives");
        validator.reference(definition.m_rewards, path + "/rewards");
    }
    for (const auto& definition : regions()) {
        const auto path = location("regions", definition);
        validator.check(definition.m_first_floor.m_value > 0 && definition.m_last_floor >= definition.m_first_floor, path + ": invalid floor range");
        validator.references(definition.m_battlefields, path + "/battlefields");
        validator.references(definition.m_enemies, path + "/enemies");
    }
    for (const auto& definition : buildings()) {
        const auto path = location("buildings", definition);
        validator.check(definition.m_maximum_level > 0, path + "/maximum_level: must be positive");
        validator.cost(definition.m_construction_cost, path + "/construction_cost");
        validator.costs(definition.m_upgrade_costs, path + "/upgrade_costs");
        validator.modifiers(definition.m_modifiers, path + "/modifiers");
        validator.unlocks(definition.m_required_unlocks, path + "/required_unlocks");
    }
    for (const auto& definition : rewards()) {
        const auto path = location("rewards", definition);
        validator.check(enum_below(definition.m_policy, 2), path + "/policy: invalid policy");
        validator.check(definition.m_rolls >= 0 && definition.m_item_rolls >= 0 && definition.m_experience_per_participant >= 0, path + ": rolls and experience must be nonnegative");
        if (definition.m_policy == Reward_roll_policy::weighted_with_replacement) {
            validator.check(definition.m_rolls == 0 || !definition.m_entries.empty(), path + "/entries: weighted rolls require entries");
        }
        validator.check(definition.m_item_rolls == 0 || !definition.m_item_entries.empty(), path + "/item_entries: item rolls require entries");
        validator.weights(definition.m_entries, path + "/entries");
        validator.weights(definition.m_item_entries, path + "/item_entries");
        for (const auto& entry : definition.m_entries) {
            validator.reference(entry.m_resource, path + "/entries/resource");
            validator.check(entry.m_minimum >= 0 && entry.m_maximum >= entry.m_minimum, path + "/entries: invalid reward amount range");
        }
        for (const auto& entry : definition.m_item_entries) {
            validator.reference(entry.m_definition, path + "/item_entries/definition");
        }
    }
    for (const auto& definition : idle_activities()) {
        const auto path = location("idle_activities", definition);
        validator.reference(definition.m_rewards, path + "/rewards");
        validator.check(definition.m_interval.count() > 0 && definition.m_offline_cap.count() >= 0, path + ": interval must be positive and offline_cap must be nonnegative");
    }
    return validator.result();
}