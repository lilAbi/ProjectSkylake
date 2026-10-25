#ifndef PROJECTSKYLAKE_DEFINITION_REGISTRY_H
#define PROJECTSKYLAKE_DEFINITION_REGISTRY_H

#include "game/core/ids.h"
#include "game/core/results.h"

#include <algorithm>
#include <span>
#include <string_view>
#include <unordered_map>
#include <vector>

inline bool valid_definition_id(std::string_view id) {
    if (id.empty() || id.size() > 128)
        return false;
    return std::ranges::all_of(
        id,
        [](const unsigned char c) {
            return (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z') ||
                   (c >= '0' && c <= '9') ||
                   c == '.' ||
                   c == '_' ||
                   c == '-';
        }
    );
}

template <typename Definition>
class DefinitionRegistry {
public:
    using ID = decltype(std::declval<Definition>().m_id);

    [[nodiscard]] static Result<DefinitionRegistry> build(std::vector<Definition> definitions, std::string_view kind);
    [[nodiscard]] const Definition*                 find(const ID& id) const;
    [[nodiscard]] std::span<const Definition>       all() const noexcept;

private:
    std::vector<Definition>                     m_definitions;
    std::unordered_map<ID, std::size_t, IDHash> m_index;
};

template <typename Definition>
Result<DefinitionRegistry<Definition>> DefinitionRegistry<Definition>::build(std::vector<Definition> definitions, const std::string_view kind) {
    //sort definitions by id
    std::ranges::sort(
        definitions,
        {},
        [](const Definition& definition) -> const auto& {
            return definition.m_id.m_value;
        }
    );
    //create a new definition registry
    DefinitionRegistry registry;
    registry.m_definitions = std::move(definitions);
    registry.m_index.reserve(registry.m_definitions.size());
    //iterate through the current imported definitions and create associated lookup entry
    for (std::size_t index = 0; index < registry.m_definitions.size(); ++index) {
        //grab a reference to the ID and check if its valid
        const auto& id = registry.m_definitions[index].m_id;
        if (!valid_definition_id(id.m_value)) {
            return Error{
                .m_code = Error_code::invalid_content,
                .m_message = std::string(kind) + ": invalid definition ID '" + id.m_value + "' (use 1-128 ASCII letters, digits, '.', '_' or '-')"
            };
        }
        //inserts the lookup entry and check if there is a duplicate
        if (!registry.m_index.emplace(id, index).second) {
            return Error{
                .m_code = Error_code::invalid_content,
                .m_message = std::string(kind) + ": duplicate definition ID '" + id.m_value + "'"
            };
        }
    }
    //return newly created definition registry
    return registry;
}

template <typename Definition>
const Definition* DefinitionRegistry<Definition>::find(const ID& id) const {
    const auto itr = m_index.find(id);
    return itr == m_index.end() ? nullptr : &m_definitions[itr->second];
}

template <typename Definition>
std::span<const Definition> DefinitionRegistry<Definition>::all() const noexcept {
    return m_definitions;
}

#endif //PROJECTSKYLAKE_DEFINITION_REGISTRY_H
