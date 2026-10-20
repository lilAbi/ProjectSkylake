#ifndef PROJECTSKYLAKE_SERVICE_CONTEXT_H
#define PROJECTSKYLAKE_SERVICE_CONTEXT_H

#include "game/core/ids.h"

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
    GameState&                 m_state;
    const ContentCatalog&      m_content;
    EventBus&            m_events;
    //const RandomStreamFactory& m_rng;
    //const IClock&              m_clock;
};

#endif //PROJECTSKYLAKE_SERVICE_CONTEXT_H
