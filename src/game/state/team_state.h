#ifndef PROJECTSKYLAKE_TEAM_STATE_H
#define PROJECTSKYLAKE_TEAM_STATE_H

#include "game/character/team.h"
#include "game/id/id.h"

#include <map>

struct TeamState {
    std::map<TeamID, Team> teams;
};

#endif //PROJECTSKYLAKE_TEAM_STATE_H
