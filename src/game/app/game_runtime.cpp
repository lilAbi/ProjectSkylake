#include "game_runtime.h"

#include "game/content/json_content_source.h"

#include <random>

Result<void> GameRuntime::initialize() {
    m_logger->print("[GameRuntime] {}() called", __FUNCTION__);

    //load content_catalog from content_data
    JSONContentSource catalog_data_content_source{"res://content/catalog.json"};
    auto              catalog_data = catalog_data_content_source.load();
    if (const auto* catalog_data_error = std::get_if<Error>(&catalog_data)) {
        m_logger->critical("[GameRuntime] catalog_data_error: {}", catalog_data_error->m_message);
        return *catalog_data_error;
    }
    m_logger->print("[GameRuntime] Catalog data loaded.");

    //initialize "ContentCatalog" with pre-defined definitions
    auto catalog = ContentCatalog::build(std::get<CatalogData>(std::move(catalog_data)));
    if (const auto* catalog_error = std::get_if<Error>(&catalog)) {
        m_logger->critical("[GameRuntime] catalog_error: {}", catalog_error->m_message);
        return *catalog_error;
    }

    //update game's content catalog
    m_content_catalog = std::move(std::get<ContentCatalog>(std::move(catalog)));
    m_logger->print("[GameRuntime] Content Catalog created.");

    //

    return std::monostate{};
}

Result<void> GameRuntime::new_game(const NewGameRequest& new_game_request) {
    m_logger->print("[GameRuntime] {} called", __FUNCTION__);
    //check if an ongoing battle is happening
    if (!m_reservations.m_battle_characters.empty()) {
        return Error{
            .m_code = Error_code::busy,
            .m_message = "Finish the active battle before replacing the profile."
        };
    }
    //validate wallet has the correct resources
    auto wallet = ResourceWallet::create_wallet(new_game_request.m_starting_resources);
    if (const auto* error = std::get_if<Error>(&wallet)) {
        return *error;
    }
    //create a new game state
    GameState new_game_state;
    new_game_state.m_profile = {
        .m_profile_id = new_game_request.m_profile_id,
        .m_display_name = new_game_request.m_display_name,
        .m_master_seed = new_game_request.m_master_seed
    };
    new_game_state.m_wallet = std::get<ResourceWallet>(std::move(wallet));
    if (!m_content_catalog.regions().empty()) {
        new_game_state.m_progression.m_region = m_content_catalog.regions().front().m_id;
    }
    //set the state
    m_game_state = std::move(new_game_state);
    m_reservations = {};
    m_profile_active = true;
    m_slot = SaveSlot{0};
    ++m_revision.m_value;
    (void)notify_on_roster_changed(m_context);
    return std::monostate{};
}

Result<void> GameRuntime::load_profile(SaveSlot) {
    m_logger->print("[GameRuntime] {} called", __FUNCTION__);
    //load previous saved game file
    return std::monostate{};
}

Result<void> GameRuntime::save_profile() {
    return std::monostate{};
}

Result<void> GameRuntime::shutdown() {
    m_logger->print("[GameRuntime] {} called", __FUNCTION__);
    //safely de-construct objects
    return std::monostate{};
}

const GameState& GameRuntime::get_state() const {
    return m_game_state;
}

const ContentCatalog& GameRuntime::get_content() const {
    return m_content_catalog;
}

EventBus& GameRuntime::get_event_bus() {
    return m_event_bus;
}

RosterService& GameRuntime::get_roster_service() {
    return m_roster_service;
}

SummoningService& GameRuntime::get_summoning_service() {
    return m_summoning_service;
}

EquipmentService& GameRuntime::get_equipment_service() {
    return m_equipment_service;
}

CharacterProgressionService& GameRuntime::get_character_progression_service() {
    return m_character_progression_service;
}

CraftingService& GameRuntime::get_crafting_service() {
    return m_crafting_service;
}

CampService& GameRuntime::get_camp_service() {
    return m_camp_service;
}

ProgressionService& GameRuntime::get_progression_service() {
    return m_progression_service;
}

BattleApplicationService& GameRuntime::get_battles_service() {
    return m_battles_service;
}

IdleProgressionService& GameRuntime::get_idle_service() {
    return m_idle_service;
}

SaveService& GameRuntime::get_save_service() {
    return m_saves_service;
}

ActivityCoordinator& GameRuntime::get_activities() {
    return m_activities;
}