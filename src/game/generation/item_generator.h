#ifndef PROJECTSKYLAKE_ITEM_GENERATOR_H
#define PROJECTSKYLAKE_ITEM_GENERATOR_H

#include "game/content/catalog.h"
#include "game/core/random.h"
#include "game/core/results.h"

#include <cstdint>

class ItemGenerator {
public:
    Result<ItemInstance> generate(ItemID, ItemDefinitionID, std::int32_t tier, const ContentCatalog&, IRandomStream&) const;
};

#endif //PROJECTSKYLAKE_ITEM_GENERATOR_H
