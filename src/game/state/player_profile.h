#ifndef PROJECTSKYLAKE_PLAYER_PROFILE_H
#define PROJECTSKYLAKE_PLAYER_PROFILE_H

#include "game/id/seed.h"

#include <string>

struct PlayerProfile {
    std::string m_profile_id;
    std::string m_display_name;
    Seed        m_master_seed;
};

#endif //PROJECTSKYLAKE_PLAYER_PROFILE_H
