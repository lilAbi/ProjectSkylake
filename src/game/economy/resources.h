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
    Result<ResourceWallet> get_wallet(const ResourceBundle& balance);
    std::int64_t           get_balance_for_resource(const ResourceID& resource_id) const;
    ResourceBundle         get_snapshot() const;
    bool                   can_afford(const ResourceCost& cost) const;
    Result<void>           try_spend(const ResourceCost& cost);
    Result<void>           grant(const ResourceBundle& resources);

private:
    //ensures the wallet has valid resource ids and positive amounts
    Result<void> validate_bundle(const ResourceBundle& resource_bundle);

private:
    std::map<ResourceID, std::int64_t> m_balances;
};

#endif //PROJECTSKYLAKE_RESOURCES_H
