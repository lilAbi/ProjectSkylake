#ifndef PROJECTSKYLAKE_INVENTORY_STATE_H
#define PROJECTSKYLAKE_INVENTORY_STATE_H

#include "game/id/id.h"
#include "game/instance/item_instance.h"

#include <map>

struct InventoryState {
    std::map<ItemID, ItemInstance> m_items;
    std::int64_t                   m_capacity{100};
};

#endif //PROJECTSKYLAKE_INVENTORY_STATE_H
