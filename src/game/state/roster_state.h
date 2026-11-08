#ifndef PROJECTSKYLAKE_ROSTER_STATE_H
#define PROJECTSKYLAKE_ROSTER_STATE_H

#include "game/app/events.h"
#include "game/app/service_context.h"
#include "game/character/character.h"
#include "game/core/ids.h"
#include "game/event/event_bus.h"

#include <map>

struct RosterState {
    std::map<CharacterID, CharacterInstance> m_characters;
};

#endif //PROJECTSKYLAKE_ROSTER_STATE_H
