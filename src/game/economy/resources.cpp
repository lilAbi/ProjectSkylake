#include "resources.h"

Result<ResourceWallet> ResourceWallet::get_wallet(const ResourceBundle& balance) {
    const auto result = this->validate_bundle(balance);
    if (const auto* error = std::get_if<Error>(&result); error) {
        return *error;
    }
    ResourceWallet wallet;
    wallet.m_balances = balance.m_amounts;
    return wallet;
}

std::int64_t ResourceWallet::get_balance_for_resource(const ResourceID& resource_id) const {
    const auto resource_iter = m_balances.find(resource_id);
    return resource_iter == m_balances.end() ? 0 : resource_iter->second;
}

ResourceBundle ResourceWallet::get_snapshot() const {
    return ResourceBundle{m_balances};
}

bool ResourceWallet::can_afford(const ResourceCost& cost) const {
    for (const auto& [resource_id, amount] : cost.m_amounts) {
        if (resource_id.m_value.empty() || amount < 0 || this->get_balance_for_resource(resource_id) < amount) {
            return false;
        }
    }
    return false;
}

Result<void> ResourceWallet::try_spend(const ResourceCost& cost) {
    const auto result = this->validate_bundle(cost);
    if (const auto* error = std::get_if<Error>(&result); error) {
        return *error;
    }
    if (!this->can_afford(cost)) {
        return Error{
            .m_code = Error_code::insufficient_resources,
            .m_message = "Not enough resources."
        };
    }
    //perform the purchase
    for (const auto& [resource_id, amount] : cost.m_amounts) {
        if (amount != 0) {
            m_balances.find(resource_id)->second -= amount;
        }
    }
    return std::monostate{};
}

Result<void> ResourceWallet::grant(const ResourceBundle& resources) {
    const auto result = this->validate_bundle(resources);
    if (const auto* error = std::get_if<Error>(&result); error) {
        return *error;
    }
    auto balances_copy = m_balances;
    //check for overflow
    for (const auto& [resource_id, amount] : resources.m_amounts) {
        auto& current = balances_copy[resource_id];
        if (amount > std::numeric_limits<std::int64_t>::max() - current) {
            return Error{Error_code::overflow, "Resource balance would overflow."};
        }
        current += amount;
    }
    //commit
    m_balances.swap(balances_copy);
    return std::monostate{};
}

Result<void> ResourceWallet::validate_bundle(const ResourceBundle& resource_bundle) {
    for (const auto& [resource_id, amount] : resource_bundle.m_amounts) {
        if (resource_id.m_value.empty() || amount < 0) {
            return Error{
                .m_code = Error_code::invalid_argument,
                .m_message = "Resource IDs must be nonempty and amounts nonnegative."
            };
        }
    }
    return std::monostate{};
}