#ifndef PROJECTSKYLAKE_RESOURCES_H
#define PROJECTSKYLAKE_RESOURCES_H

#include "game/core/ids.h"
#include "game/core/results.h"

#include <map>

struct ResourceBundle {
    std::map<ResourceID, std::int64_t> m_amounts;
};

// All values >= 0.
using ResourceCost = ResourceBundle;

struct ResourceDelta {
    std::map<ResourceID, std::int64_t> m_amounts;
};

// Signed changes.
class ResourceWallet {
public:
    static Result<ResourceWallet> from_balances(const ResourceBundle&);
    std::int64_t                  balance(const ResourceID&) const;
    ResourceBundle                snapshot() const;
    bool                          can_afford(const ResourceCost&) const;
    Result<void>                  try_spend(const ResourceCost&);
    Result<void>                  grant(const ResourceBundle&);

private:
    std::map<ResourceID, std::int64_t> m_balances;
};

#endif //PROJECTSKYLAKE_RESOURCES_H
