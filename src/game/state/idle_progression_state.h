#ifndef PROJECTSKYLAKE_IDLE_PROGRESSION_STATE_H
#define PROJECTSKYLAKE_IDLE_PROGRESSION_STATE_H

#include "idle_assignment.h"
#include "game/id/resource_bundle.h"

struct IdleProgressionState {
    std::vector<IdleAssignment> m_assignments;
    ResourceBundle              m_pending;
};

#endif //PROJECTSKYLAKE_IDLE_PROGRESSION_STATE_H
