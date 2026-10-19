#ifndef PROJECTSKYLAKE_SERVICE_CONTEXT_H
#define PROJECTSKYLAKE_SERVICE_CONTEXT_H

#include "game/core/ids.h"

#include <set>

class ContentCatalog;
class DomainEventBus;
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
    Revision&                  m_revision;
    const ContentCatalog&      m_content;
    DomainEventBus&            m_events;
    const RandomStreamFactory& m_rng;
    const IClock&              m_clock;
    ActivityReservations&      m_reservations;
};

#endif //PROJECTSKYLAKE_SERVICE_CONTEXT_H
