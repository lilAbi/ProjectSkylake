#include "json_content_source.h"

#include "json_helper.h"

#include <godot_cpp/classes/file_access.hpp>

Result<std::string> JSONContentSource::read_godot_content_file(const std::string& path) {
    //open file godot file access api
    const auto file = godot::FileAccess::open(godot::String::utf8(path.c_str()), godot::FileAccess::READ);
    //check if file was opened
    if (file.is_null()) {
        return Error{
            .m_code = Error_code::io_failure,
            .m_message = path + ": cannot open content file (Godot error " + std::to_string(godot::FileAccess::get_open_error()) + ")"
        };
    }
    const auto bytes = file->get_buffer(static_cast<std::int64_t>(file->get_length()));
    if (static_cast<std::uint64_t>(bytes.size()) != file->get_length()) {
        return Error{
            .m_code = Error_code::io_failure,
            .m_message = path + ": short read while loading content"
        };
    }
    return bytes.is_empty() ? std::string{} : std::string{reinterpret_cast<const char*>(bytes.ptr()), static_cast<std::size_t>(bytes.size())};
}

Result<CatalogData> JSONContentSource::load() {
    if (m_manifest_path.empty()) {
        return Error{
            .m_code = Error_code::not_configured,
            .m_message = "JSON content source requires a manifest path and file reader"
        };
    }
    try {
        //attempt to read the content manifest file into a string buffer
        const auto manifest_read_result = JSONContentSource::read_godot_content_file(m_manifest_path);
        if (const auto* manifest_read_error = std::get_if<Error>(&manifest_read_result)) {
            throw Error{*manifest_read_error};
        }
        //attempt to extract a string result from the buffer
        const auto& manifest_string_buffer = std::get<std::string>(manifest_read_result);
        //parse manifest into a json object
        const auto manifest = parse_json(manifest_string_buffer);
        //check if version is compatible
        schema_version(manifest);
        const Object             object{manifest, "#", {"schema_version", "content_version", "sources"}};
        CatalogData              catalog_data;
        std::vector<std::string> sources;
        object.field("content_version", catalog_data.m_content_version, true);
        object.field("sources", sources, true);

        if (catalog_data.m_content_version.empty()) {
            fail("#/content_version", "must not be empty");
        }

        if (sources.size() > max_sources) {
            fail("#/sources", "manifest exceeds source count limit");
        }

        // Resolve and check the complete manifest before opening source files.
        std::vector<std::string> paths;
        std::set<std::string>    seen;
        for (std::size_t i = 0; i < sources.size(); ++i) {
            const auto path = resolve_source(m_manifest_path, sources[i], "#/sources/" + std::to_string(i));
            if (path == m_manifest_path || !seen.insert(path).second) {
                fail("#/sources/" + std::to_string(i), "duplicate source or manifest self-reference '" + path + "'");
            }
            paths.push_back(path);
        }
        std::size_t definition_count = 0;
        for (const auto& path : paths) {
            //load_definitions(data, read_json(path), definition_count);
        }
        return catalog_data;
    } catch (const Error& failure) {
        auto error = failure;
        if (error.m_message.starts_with('#')) {
            error.m_message = m_manifest_path + error.m_message;
        }
        return error;
    } catch (const Json::exception& error) {
        return Error{
            .m_code = Error_code::invalid_content,
            .m_message = m_manifest_path + ": " + error.what()
        };
    }
}