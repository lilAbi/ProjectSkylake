#ifndef PROJECTSKYLAKE_JSON_DEFINITIONS_H
#define PROJECTSKYLAKE_JSON_DEFINITIONS_H

#include <nlohmann/json_fwd.hpp>

#include <string_view>

struct CatalogData;

namespace content::json_detail {

    // Internal decoder dispatch; contextual errors are caught by append_source().
    void decode_definitions(CatalogData& data, std::string_view kind, const nlohmann::json& definitions);

}

#endif //PROJECTSKYLAKE_JSON_DEFINITIONS_H
