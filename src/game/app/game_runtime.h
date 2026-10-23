#ifndef PROJECTSKYLAKE_GAME_RUNTIME_H
#define PROJECTSKYLAKE_GAME_RUNTIME_H

#include "activity_coordinator.h"
#include "battle_application_service.h"
#include "camp_service.h"
#include "character_progression_service.h"
#include "crafting_service.h"
#include "equipment_service.h"
#include "requests.h"
#include "save_service.h"
#include "service_context.h"
#include "summoning_service.h"
#include "idle_progression_service.h"
#include "progression_service.h"
#include "roster_service.h"
#include "game/content/catalog.h"
#include "game/content/content_source.h"
#include "game/core/results.h"
#include "game/core/types.h"
#include "game/event/event_bus.h"
#include "game/generation/character_generation.h"
#include "game/generation/encounter_generation.h"
#include "game/generation/item_generator.h"
#include "game/generation/reward_generator.h"
#include "game/state/game_state.h"
#include "util/console.hpp"

class GameRuntime {
    friend class SceneCoordinator;

public:
    GameRuntime() = default;
    ~GameRuntime() = default;

    Result<void> initialize();
    Result<void> new_game(const NewGameRequest& new_game_request);
    Result<void> load_profile(SaveSlot);
    Result<void> shutdown();

    const GameState&             get_state() const;
    const ContentCatalog&        get_content() const;
    EventBus&                    get_event_bus();
    RosterService&               get_roster_service();
    SummoningService&            get_summoning_service();
    EquipmentService&            get_equipment_service();
    CharacterProgressionService& get_character_progression_service();
    CraftingService&             get_crafting_service();
    CampService&                 get_camp_service();
    ProgressionService&          get_progression_service();
    BattleApplicationService&    get_battles_service();
    IdleProgressionService&      get_idle_service();
    SaveService&                 get_save_service();
    ActivityCoordinator&         get_activities();

private:
    Logger*              m_logger = Logger::get();
    GameState            m_state{};
    ContentCatalog       m_content{};
    EventBus             m_event_bus{};
    ActivityReservations m_reservations{};
    ServiceContext       m_context{.m_state = m_state, .m_content = m_content, .m_events = m_event_bus};
    // Domain/application helpers.
    CharacterGenerator m_character_generator{m_content};
    EncounterGenerator m_encounter_generator{m_content};
    ItemGenerator      m_item_generator;
    RewardGenerator    m_reward_generator;
    // Application services.
    RosterService               m_roster_service{m_context};
    SummoningService            m_summoning_service{m_context, m_character_generator};
    EquipmentService            m_equipment_service{m_context};
    CharacterProgressionService m_character_progression_service{m_context};
    CraftingService             m_crafting_service{m_context, m_item_generator};
    CampService                 m_camp_service{m_context};
    ProgressionService          m_progression_service{m_context};
    BattleApplicationService    m_battles_service{m_context, m_encounter_generator, m_reward_generator};
    IdleProgressionService      m_idle_service{m_context};
    SaveService                 m_saves_service;
    ActivityCoordinator         m_activities{m_progression_service, m_roster_service, m_battles_service};
    // metadata
    bool m_profile_active = false;
};

#endif //PROJECTSKYLAKE_GAME_RUNTIME_H
