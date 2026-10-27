#ifndef PROJECTSKYLAKE_CATALOG_INSPECTION_H
#define PROJECTSKYLAKE_CATALOG_INSPECTION_H

#include "game/content/catalog.h"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

#include <string_view>

namespace debug {

    enum class Catalog_kind : std::size_t {
        classes,
        traits,
        abilities,
        statuses,
        items,
        recipes,
        banners,
        resources,
        enemies,
        battlefields,
        objectives,
        encounters,
        regions,
        buildings,
        rewards,
        idle_activities
    };

    struct CatalogCategory {
        Catalog_kind      m_kind;
        std::string_view m_name;
        std::string_view m_cpp_type;
    };

    // IDs are unique within a registry, not necessarily across registry types.
    struct CatalogKey {
        Catalog_kind m_kind{};
        std::string m_id;
        auto        operator<=>(const CatalogKey&) const = default;
    };

    struct CatalogReference {
        CatalogKey  m_target;
        std::string m_path;           // JSON pointer into the resolved definition.
        bool        m_on_key = false; // ResourceCost IDs occur as object keys.
    };

    struct CatalogUnlock {
        std::string m_id;
        std::string m_path;
    };

    struct CatalogEntry {
        CatalogKey                    m_key;
        std::string                   m_name; // Empty when the type has no name.
        nlohmann::ordered_json        m_fields;
        std::vector<CatalogReference> m_references;
        std::vector<CatalogUnlock>    m_unlocks;
    };

    struct CatalogUse {
        CatalogKey  m_source;
        std::string m_path;
    };

    // Owns every value. No spans or pointers into GameRuntime survive configure().
    struct CatalogSnapshot {
        std::string                                   m_content_version;
        std::vector<CatalogEntry>                     m_entries;
        std::map<CatalogKey, std::size_t>             m_index;
        std::map<CatalogKey, std::vector<CatalogUse>> m_used_by;

        [[nodiscard]] const CatalogEntry* find(const CatalogKey& key) const;
    };

    [[nodiscard]] std::span<const CatalogCategory> catalog_categories() noexcept;
    [[nodiscard]] CatalogSnapshot                  inspect_catalog(const ContentCatalog& catalog);

}

#endif //PROJECTSKYLAKE_CATALOG_INSPECTION_H
