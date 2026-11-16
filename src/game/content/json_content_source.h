#ifndef PROJECTSKYLAKE_JSON_CONTENT_SOURCE_H
#define PROJECTSKYLAKE_JSON_CONTENT_SOURCE_H

#include "content_source.h"

#include <cstddef>
#include <functional>
#include <string>
#include <utility>

class JSONContentSource final : public IContentSource {
public:
    //a reader must reject files larger than max_bytes before allocating their contents.
    using FileReader = std::function<Result<std::string>(const std::string& path, std::size_t max_bytes)>;
    //uses Godot FileAccess, including res:// and user:// support.
    explicit JSONContentSource(std::string manifest_path);
    //allows the same loader to run with an in-memory or ordinary filesystem reader.
    JSONContentSource(std::string manifest_path, FileReader file_reader) : m_manifest_path(std::move(manifest_path)), m_file_reader(std::move(file_reader)) {}
    [[nodiscard]] static Result<std::string> read_file(const std::string& path);
    [[nodiscard]] Result<CatalogData>        load() override;

private:
    std::string m_manifest_path;
    FileReader  m_file_reader;
};

#endif //PROJECTSKYLAKE_JSON_CONTENT_SOURCE_H
