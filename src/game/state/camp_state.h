#ifndef PROJECTSKYLAKE_CAMP_STATE_H
#define PROJECTSKYLAKE_CAMP_STATE_H

#include "building_instance.h"
#include "game/id/id.h"

#include <map>

struct CampState {
    std::map<BuildingID, BuildingInstance> m_buildings;
};

#endif //PROJECTSKYLAKE_CAMP_STATE_H
