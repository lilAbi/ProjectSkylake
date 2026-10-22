#ifndef PROJECTSKYLAKE_JSON_CONTENT_SOURCE_H
#define PROJECTSKYLAKE_JSON_CONTENT_SOURCE_H

#include "catalog_data.h"
#include "content_source.h"

#include <cstddef>
#include <functional>
#include <string>

// The reader supplies bytes, whether from Godot's PCK filesystem or a test.
// It must enforce max_bytes before allocating the file buffer.
using ContentFileReader = std::function<Result<std::string>(const std::string& path, std::size_t max_bytes)>;

class JSONContentSource final : public IContentSource {
public:
    JSONContentSource(std::string manifest_path, ContentFileReader reader)
        : m_manifest_path(std::move(manifest_path)), m_reader(std::move(reader)) {};

    [[nodiscard]] Result<CatalogData> load() override {};

private:
    std::string       m_manifest_path;
    ContentFileReader m_reader;
};

#endif //PROJECTSKYLAKE_JSON_CONTENT_SOURCE_H
