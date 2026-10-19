#ifndef PROJECTSKYLAKE_ITEM_H
#define PROJECTSKYLAKE_ITEM_H

#include "game/character/stats.h"
#include "game/core/ids.h"
#include "game/core/types.h"

#include <vector>

struct GeneratedItem {
    ItemDefinitionID          m_definition;
    std::int32_t              m_level{1};
    std::vector<StatModifier> m_affixes;
    GenerationMetadata        m_generation;
};

struct ItemInstance {
    ItemID                    m_id;
    ItemDefinitionID          m_definition;
    std::int32_t              m_level{1};
    std::vector<StatModifier> m_affixes;
    GenerationMetadata        m_generation;
};

#endif //PROJECTSKYLAKE_ITEM_H
