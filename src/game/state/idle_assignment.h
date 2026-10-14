#ifndef PROJECTSKYLAKE_IDLE_ASSIGNMENT_H
#define PROJECTSKYLAKE_IDLE_ASSIGNMENT_H

#include "game/id/id.h"

#include <chrono>

struct IdleAssignment {
    IdleAssignmentID         m_id;
    TeamID                   m_team;
    IdleActivityID           m_activity;
    std::chrono::sys_seconds m_checkpoint;
};

#endif //PROJECTSKYLAKE_IDLE_ASSIGNMENT_H
