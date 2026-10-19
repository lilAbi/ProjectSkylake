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
    CraftingService(ServiceContext&, const ItemGenerator&);
    std::vector<RecipeView> recipes() const;
    Result<CraftResult>     craft(const CraftRequest&);
    Result<void>            upgrade(ItemID);

private:
    ServiceContext&      m_context;
    const ItemGenerator& m_generator;
};

#endif //PROJECTSKYLAKE_CRAFTING_SERVICE_H
