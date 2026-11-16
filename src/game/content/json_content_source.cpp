#include "json_content_source.h"

#include "json_decoding.h"

#include <algorithm>
#include <variant>

Result<CatalogData> JSONContentSource::load() {
    using namespace content::json_detail;
    //validate if manifest and reader obj exist
    if (m_manifest_path.empty() || !m_file_reader) {
        return Error{
            .m_code = Error_code::not_configured,
            .m_message = "JSON content source requires a manifest path and file reader"
        };
    }
    //the budget includes the manifest and is passed to the reader before every read.
    std::size_t total_bytes = 0;
    //local file-reading helper lamda to enforces size constraints and tracks how much data has been loaded
    const auto read = [&](const std::string& path) -> Result<std::string> {
        const auto limit = std::min(max_file_bytes, max_total_bytes - total_bytes);
        auto       result = m_file_reader(path, limit);
        if (const auto* bytes = std::get_if<std::string>(&result)) {
            // Also enforce the contract when a custom reader returns too much data.
            if (bytes->size() > limit) {
                return Error{
                    .m_code = Error_code::invalid_content,
                    .m_message = path + ": content byte limit exceeded"
                };
            }
            total_bytes += bytes->size();
        }
        return result;
    };

    CatalogData              data;
    std::vector<std::string> paths;
    {
        auto bytes = read(m_manifest_path);
        if (const auto* error = std::get_if<Error>(&bytes)) {
            return *error;
        }
        auto manifest = decode_manifest(std::get<std::string>(bytes), m_manifest_path);
        if (const auto* error = std::get_if<Error>(&manifest)) {
            return *error;
        }
        auto& decoded = std::get<Manifest>(manifest);
        data.m_content_version = std::move(decoded.m_content_version);
        paths = std::move(decoded.m_sources);
    }

    std::size_t definition_count = 0;
    for (const auto& path : paths) {
        auto bytes = read(path);
        if (const auto* error = std::get_if<Error>(&bytes)) {
            return *error;
        }
        auto result = append_source(data, std::get<std::string>(bytes), path, definition_count);
        if (const auto* error = std::get_if<Error>(&result)) {
            return *error;
        }
    }
    return data;
}
