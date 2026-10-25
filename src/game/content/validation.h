#ifndef PROJECTSKYLAKE_VALIDATION_H
#define PROJECTSKYLAKE_VALIDATION_H
#include "catalog.h"
#include "game/core/results.h"

#include <set>
#include <utility>

template <typename Enum>
bool enum_below(Enum value, int count) {
    const auto number = std::to_underlying(value);
    return std::cmp_greater_equal(number, 0) && std::cmp_less(number, count);
}

template <typename Definition>
std::string location(const char* kind, const Definition& definition) {
    return std::string(kind) + "/" + definition.m_id.m_value;
}

class Validator {
public:
    explicit Validator(const ContentCatalog& catalog) : m_catalog(catalog) {}

    void check(bool condition, const std::string& message);

    template <typename ID>
    void reference(const ID& id, const std::string& path);

    template <typename ID>
    void references(const std::vector<ID>& ids, const std::string& path);

    void unlocks(const std::vector<UnlockID>& ids, const std::string& path);

    void cost(const ResourceCost& cost, const std::string& path);

    void costs(const std::vector<ResourceCost>& costs, const std::string& path);

    void stats(const StatBlock& stats, const std::string& path);

    void modifiers(const std::vector<StatModifier>& modifiers, const std::string& path);

    void effects(const std::vector<EffectDefinition>& effects, const std::string& path);

    void battlefield(const BattlefieldTemplate& battlefield, const std::string& path);

    template <typename Entry>
    void weights(const std::vector<Entry>& entries, const std::string& path);

    Result<void> result() const;

private:
    const ContentCatalog& m_catalog;
    std::optional<Error>  m_error;
};

template <typename ID>
void Validator::reference(const ID& id, const std::string& path) {
    check(m_catalog.find(id) != nullptr, path + ": unknown definition '" + id.m_value + "'");
}

template <typename ID>
void Validator::references(const std::vector<ID>& ids, const std::string& path) {
    for (std::size_t i = 0; i < ids.size(); ++i) {
        reference(ids[i], path + "/" + std::to_string(i));
    }
}

template <typename Entry>
void Validator::weights(const std::vector<Entry>& entries, const std::string& path) {
    std::uint64_t total = 0;
    for (const auto& entry : entries) {
        check(entry.m_weight > 0, path + ": weight must be positive");
        const bool fits = entry.m_weight <= std::numeric_limits<std::uint64_t>::max() - total;
        check(fits, path + ": total weight overflows uint64");
        if (fits) {
            total += entry.m_weight;
        }
    }
}

#endif //PROJECTSKYLAKE_VALIDATION_H
