#ifndef PROJECTSKYLAKE_CRAFTING_SERVICE_H
#define PROJECTSKYLAKE_CRAFTING_SERVICE_H

#include "requests.h"
#include "views.h"
#include "game/core/results.h"

#include <vector>

struct ServiceContext;
class ItemGenerator;
struct CraftRequest;

class CraftingService {
public:
    CraftingService(ServiceContext& context, const ItemGenerator& item_generator) : m_context(context), m_item_generator(item_generator) {};
    ~CraftingService() = default;
    std::vector<RecipeView> recipes() const;
    Result<CraftResult>     craft(const CraftRequest&);
    Result<void>            upgrade(ItemID);

private:
    const ServiceContext& m_context;
    const ItemGenerator&  m_item_generator;
};

#endif //PROJECTSKYLAKE_CRAFTING_SERVICE_H
