#ifndef PROJECTSKYLAKE_RESOURCE_WALLET_H
#define PROJECTSKYLAKE_RESOURCE_WALLET_H
#include "game/error/error.h"
#include "game/id/resource_bundle.h"

#include <cstdint>

class ResourceWallet {
public:
    static Result<ResourceWallet> from_balances(const ResourceBundle&);
    std::int64_t                  balance(const ResourceID&) const;
    ResourceBundle                snapshot() const;
    bool                          can_afford(const ResourceCost&) const;
    Result<void>                  try_spend(const ResourceCost&);
    Result<void>                  grant(const ResourceBundle&);

private:
    std::map<ResourceID, std::int64_t> m_balance;
};

#endif //PROJECTSKYLAKE_RESOURCE_WALLET_H
