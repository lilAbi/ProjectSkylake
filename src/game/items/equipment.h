#ifndef PROJECTSKYLAKE_EQUIPMENT_H
#define PROJECTSKYLAKE_EQUIPMENT_H

#include "game/core/ids.h"

#include <map>

enum class Equipment_slot {
    main_hand,
    off_hand,
    helmet,
    chest,
    gloves,
    pants,
    boots,
    ear_ring_1,
    ear_ring_2,
    accessory_1,
    accessory_2,
    max_equipment_slot_count
};

struct Equipment {
    std::map<Equipment_slot, ItemID> m_slots;
};

#endif //PROJECTSKYLAKE_EQUIPMENT_H
