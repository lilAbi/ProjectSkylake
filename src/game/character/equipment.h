#ifndef PROJECTSKYLAKE_EQUIPMENT_H
#define PROJECTSKYLAKE_EQUIPMENT_H

#include "game/id/id.h"

#include <map>

enum class Equipment_slot {
    main_hand,
    off_hand,
    armor,
    accessory
};

struct Equipment {
    std::map<Equipment_slot, ItemID> m_slots;
};

#endif //PROJECTSKYLAKE_EQUIPMENT_H
