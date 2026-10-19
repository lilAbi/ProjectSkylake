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
#include "game/core/results.h"
#include "game/core/types.h"
#include "game/event/event_bus.h"
#include "game/generation/character_generation.h"
#include "game/generation/encounter_generation.h"
#include "game/generation/item_generator.h"
#include "game/generation/reward_generator.h"
#include "game/state/game_state.h"

class GameRuntime {
public:
    GameRuntime() = default;
    ~GameRuntime() = default;

    Result<void> initialize();
    /*
    Result<void> new_game(const NewGameRequest&);
    Result<void> load_profile(SaveSlot);
    Result<void> shutdown();

    Revision                     get_revision() const;
    ActivityGeneration           get_generation() const;
    const GameState&             get_state() const;
    const ContentCatalog&        get_content() const;
    DomainEventBus&              get_event_bus();
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
    ActivityCoordinator&         get_activities_service();
    */

private:
    ContentCatalog       m_content{{}};
    /*
    GameState            m_state{};
    Revision             m_revision{};
    ActivityGeneration   m_generation{};
    EventBus             m_events{};
    ActivityReservations m_reservations{};
    ServiceContext       m_context;
    // Domain/application helpers.
    CharacterGenerator m_character_generator;
    EncounterGenerator m_encounter_generator;
    ItemGenerator      m_item_generator;
    RewardGenerator    m_reward_generator;
    // Application services.
    RosterService               m_roster_service;
    SummoningService            m_summoning_service;
    EquipmentService            m_equipment_service;
    CharacterProgressionService m_character_progression_service;
    CraftingService             m_crafting_service;
    CampService                 m_camp_service;
    ProgressionService          m_progression_service;
    BattleApplicationService    m_battles_service;
    IdleProgressionService      m_idle_service;
    SaveService                 m_saves_service;
    ActivityCoordinator         m_activities;
    // metadata
    bool m_profile_active = false;
    */
};

#endif //PROJECTSKYLAKE_GAME_RUNTIME_H
