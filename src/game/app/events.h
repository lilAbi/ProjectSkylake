#ifndef PROJECTSKYLAKE_GAME_EVENTS_H
#define PROJECTSKYLAKE_GAME_EVENTS_H

#include "game/core/types.h"
#include "game/economy/resources.h"
#include <vector>

struct RosterChanged {
    Revision m_revision;
};

struct CharactersSummoned {
    BannerID                 m_banner;
    std::vector<CharacterID> m_characters;
    ResourceCost             m_spent;
    Revision                 m_revision;
};

#endif //PROJECTSKYLAKE_EVENTS_H
