#ifndef PROJECTSKYLAKE_RESOURCE_BUNDLE_H
#define PROJECTSKYLAKE_RESOURCE_BUNDLE_H

#include "id.h"

#include <map>

struct ResourceBundle {
    std::map<ResourceID, std::int64_t> m_amounts;
};

using ResourceDelta = ResourceBundle;
using ResourceCost = ResourceBundle;

#endif //PROJECTSKYLAKE_RESOURCE_BUNDLE_H
