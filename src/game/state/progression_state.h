#ifndef PROJECTSKYLAKE_PROGRESSION_STATE_H
#define PROJECTSKYLAKE_PROGRESSION_STATE_H

#include "game/id/floor_number.h"
#include "game/id/id.h"

#include <set>

struct ProgressionState {
    FloorNumber        m_highest_completed{0};
    RegionID           m_region;
    std::set<UnlockID> m_unlocks;
};

#endif //PROJECTSKYLAKE_PROGRESSION_STATE_H
