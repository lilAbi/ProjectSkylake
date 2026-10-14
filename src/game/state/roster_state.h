#ifndef PROJECTSKYLAKE_ROSTER_STATE_H
#define PROJECTSKYLAKE_ROSTER_STATE_H

#include "game/character/character_instance.h"
#include "game/id/id.h"

#include <map>

struct RosterState {
    std::map<CharacterID, CharacterInstance> characters;
};

#endif //PROJECTSKYLAKE_ROSTER_STATE_H
