#ifndef PROJECTSKYLAKE_BUILDING_INSTANCE_H
#define PROJECTSKYLAKE_BUILDING_INSTANCE_H

#include "game/id/id.h"

struct BuildingTag;
using BuildingID = EntityID<BuildingTag>;

struct BuildingDefinitionTag;
using BuildingDefinitionID = DefinitionID<BuildingDefinitionTag>;

struct BuildingInstance {
    BuildingID           m_id;
    BuildingDefinitionID m_definition;
    std::int32_t         n_level{1};
};

#endif //PROJECTSKYLAKE_BUILDING_INSTANCE_H
