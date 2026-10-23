#ifndef PROJECTSKYLAKE_JSON_CONTENT_SOURCE_H
#define PROJECTSKYLAKE_JSON_CONTENT_SOURCE_H

#include "catalog_data.h"
#include "content_source.h"

#include <cstddef>
#include <functional>
#include <string>

class JSONContentSource final : public IContentSource {
public:
    JSONContentSource(std::string manifest_path) : m_manifest_path(std::move(manifest_path)) {};

    [[nodiscard]] static Result<std::string> read_godot_content_file(const std::string& path);

    [[nodiscard]] Result<CatalogData> load() override;

private:
    std::string       m_manifest_path;
};

#endif //PROJECTSKYLAKE_JSON_CONTENT_SOURCE_H
