#ifndef PROJECTSKYLAKE_ITEM_INSTANCE_H
#define PROJECTSKYLAKE_ITEM_INSTANCE_H

#include "game/character/stats.h"
#include "game/id/id.h"
#include "game/id/seed.h"

#include <vector>

struct ItemInstance {
    ItemID                    m_id;
    ItemDefinitionID          m_definition;
    std::int32_t              m_level{1};
    std::vector<StatModifier> m_affixes;
    GenerationMetadata        m_generation;
};

#endif //PROJECTSKYLAKE_ITEM_INSTANCE_H
