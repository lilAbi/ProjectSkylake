#ifndef PROJECTSKYLAKE_SERVICE_CONTEXT_H
#define PROJECTSKYLAKE_SERVICE_CONTEXT_H

#include "game/core/ids.h"
#include "game/core/results.h"

#include <set>

class ContentCatalog;
class EventBus;
class IClock;
class RandomStreamFactory;
struct GameState;
struct Revision;

// Services borrow this stable context. Never cache references into its containers.
struct ActivityReservations {
    std::set<CharacterID> m_battle_characters;
    std::set<CharacterID> m_idle_characters;
};

struct ServiceContext {
    GameState&                 m_game_state;
    const ContentCatalog&      m_content_catalog;
    EventBus&                  m_events;
    Revision&                  m_revision;
    const ActivityReservations m_reservations;
    //const RandomStreamFactory& m_rng;
    //const IClock&              m_clock;
};

//state changes can only be done on the thread owned by the event bus & requires an active profile
Result<void> check_service_mutation(const ServiceContext& context);

//notify that the roster has been changed
[[nodiscard]] bool notify_on_roster_changed(const ServiceContext& context) noexcept;

#endif //PROJECTSKYLAKE_SERVICE_CONTEXT_H
