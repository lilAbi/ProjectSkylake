#ifndef PROJECTSKYLAKE_GENERATED_ITEM_H
#define PROJECTSKYLAKE_GENERATED_ITEM_H

#include "game/character/stats.h"
#include "game/id/id.h"
#include "game/id/seed.h"

#include <vector>

struct GeneratedItem {
    ItemDefinitionID          m_definition;
    std::int32_t              m_level{1};
    std::vector<StatModifier> m_affixes;
    GenerationMetadata        m_generation;
};

#endif //PROJECTSKYLAKE_GENERATED_ITEM_H
