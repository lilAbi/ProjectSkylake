#ifndef PROJECTSKYLAKE_TEAM_H
#define PROJECTSKYLAKE_TEAM_H

#include "game/core/ids.h"

#include <optional>
#include <vector>

struct Team {
    TeamID                                  m_id;
    std::string                             m_name;
    std::vector<std::optional<CharacterID>> m_slots;
};

#endif //PROJECTSKYLAKE_TEAM_H
