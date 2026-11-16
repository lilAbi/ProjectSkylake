#ifndef PROJECTSKYLAKE_JSON_DECODING_H
#define PROJECTSKYLAKE_JSON_DECODING_H

#include "catalog_data.h"
#include "game/core/results.h"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

// Internal file-format boundary. No JSON library types escape into the catalog API.
namespace content::json_detail {

    inline constexpr std::size_t max_file_bytes = 16 * 1024 * 1024;
    inline constexpr std::size_t max_total_bytes = 64 * 1024 * 1024;
    inline constexpr std::size_t max_sources = 256;
    inline constexpr std::size_t max_definitions = 100'000;
    inline constexpr int         max_json_depth = 64;

    struct Manifest {
        std::string              m_content_version;
        std::vector<std::string> m_sources; // Resolved paths, checked for duplicates and self-reference.
    };

    [[nodiscard]] Result<Manifest> decode_manifest(std::string_view text, const std::string& file_path);
    [[nodiscard]] Result<void>     append_source(CatalogData& data, std::string_view text, const std::string& file_path, std::size_t& definition_count);

}

#endif //PROJECTSKYLAKE_JSON_DECODING_H
