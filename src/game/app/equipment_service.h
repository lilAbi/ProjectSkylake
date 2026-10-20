#ifndef PROJECTSKYLAKE_EQUIPMENT_SERVICE_H
#define PROJECTSKYLAKE_EQUIPMENT_SERVICE_H

#include "views.h"
#include "game/core/results.h"

struct ServiceContext;

class EquipmentService {
public:
    explicit EquipmentService(ServiceContext& context) : m_context(context) {};
    ~EquipmentService() = default;
    InventoryView inventory() const;
    Result<void> equip(CharacterID, ItemID, Equipment_slot);
    Result<void> unequip(CharacterID, Equipment_slot);
private:
    ServiceContext& m_context;
};

#endif //PROJECTSKYLAKE_EQUIPMENT_SERVICE_H
