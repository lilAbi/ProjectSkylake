#include "json_decoding.h"

#include "json_reader.h"
#include "json_definitions.h"

#include <set>
#include <utility>

namespace content::json_detail {

    namespace {

        void schema_version(const Json& value) {
            if (!value.is_object() || !value.contains("schema_version")) {
                fail("#/schema_version", "missing required field");
            }
            const auto version = decode<std::int32_t>(value.at("schema_version"), "#/schema_version");
            if (version != 1) {
                fail("#/schema_version", "unsupported schema version " + std::to_string(version), Error_code::unsupported_version);
            }
        }

        Json parse_json(const std::string_view text) {
            std::vector<std::set<std::string>> object_keys;

            const auto callback = [&](const int depth, const Json::parse_event_t event, const Json& parsed) {
                if (depth > max_json_depth) {
                    fail("#", "JSON nesting exceeds depth limit");
                }
                if (event == Json::parse_event_t::object_start) {
                    object_keys.emplace_back();
                } else if (event == Json::parse_event_t::key) {
                    const auto key = parsed.get<std::string>();
                    if (!object_keys.back().insert(key).second) {
                        fail("#", "duplicate object key '" + key + "'");
                    }
                } else if (event == Json::parse_event_t::object_end) {
                    object_keys.pop_back();
                }
                return true;
            };

            return Json::parse(text, callback);
        }

        //takes a filename listed in the catalog manifest, validates its format, then produces the path the file reader should open
        std::string resolve_source(const std::string& manifest_file_path, const std::string& file_path, const std::string& json_path) {
            //check if path is empty or contains invalid characters
            if (file_path.empty() || file_path.find_first_of("\\\r\n?#") != std::string::npos) {
                fail(json_path, "source must be a nonempty, forward-slash file path");
            }
            //test if we are using a "godot" path
            const bool virtual_path = file_path.starts_with("res://") || file_path.starts_with("user://");
            //path cannot have a ":" unless its a godot prefix or cannot start with a "/"
            if ((!virtual_path && file_path.find(':') != std::string::npos) || file_path.front() == '/') {
                fail(json_path, "source must be relative to the manifest or use res:// or user://");
            }
            const auto  local_path = virtual_path ? file_path.substr(file_path.find("://") + 3) : file_path;
            std::size_t start = 0;
            //examines each slash-separated component and ensure its a valid format
            while (start <= local_path.size()) {
                const auto slash = local_path.find('/', start);
                const auto part = local_path.substr(start, slash == std::string::npos ? slash : slash - start);
                if (part.empty() || part == "." || part == "..") {
                    fail(json_path, "empty, '.' and '..' path components are not allowed");
                }
                if (slash == std::string::npos) {
                    break;
                }
                start = slash + 1;
            }
            if (virtual_path) {
                return file_path;
            }
            //If the manifest has no slash, there is no directory prefix to prepend.
            const auto slash = manifest_file_path.rfind('/');
            return (slash == std::string::npos ? std::string{} : manifest_file_path.substr(0, slash + 1)) + file_path;
        }

        void load_definitions(CatalogData& data, const Json& source, std::size_t& definition_count) {
            schema_version(source);
            ObjectReader object{source, "#", {"schema_version", "definitions"}};
            std::string  kind;
            object.required("kind", kind);
            object.finish();
            if (!source.contains("definitions") || !source.at("definitions").is_array()) {
                fail("#/definitions", "expected an array of definitions");
            }
            const auto& definitions = source.at("definitions");
            if (definition_count > max_definitions || definitions.size() > max_definitions - definition_count) {
                fail("#/definitions", "catalog exceeds definition limit");
            }
            decode_definitions(data, kind, definitions);
            definition_count += definitions.size();
        }

        template <typename T, typename Decode>
        Result<T> decode_document(const std::string_view text, const std::string& file_path, Decode&& decode) {
            try {
                return decode(parse_json(text));
            } catch (const Error& failure) {
                auto error = failure;
                if (error.m_message.starts_with('#')) {
                    error.m_message = file_path + error.m_message;
                }
                return error;
            } catch (const Json::exception& error) {
                return Error{
                    .m_code = Error_code::invalid_content,
                    .m_message = file_path + ": " + error.what()
                };
            }
        }

    }

    Result<Manifest> decode_manifest(const std::string_view text, const std::string& file_path) {
        return decode_document<Manifest>(text, file_path, [&](const Json& manifest) {
            schema_version(manifest);
            ObjectReader             object{manifest, "#", {"schema_version"}};
            Manifest                 result;
            std::vector<std::string> sources;
            object.required("content_version", result.m_content_version);
            object.required("sources", sources);
            object.finish();
            if (result.m_content_version.empty()) {
                fail("#/content_version", "must not be empty");
            }
            if (sources.size() > max_sources) {
                fail("#/sources", "manifest exceeds source count limit");
            }

            // Validate the complete manifest before any source file is opened.
            std::set<std::string> seen;
            result.m_sources.reserve(sources.size());
            for (std::size_t i = 0; i < sources.size(); ++i) {
                const auto location = "#/sources/" + std::to_string(i);
                auto       path = resolve_source(file_path, sources[i], location);
                if (path == file_path || !seen.insert(path).second) {
                    fail(location, "duplicate source or manifest self-reference '" + path + "'");
                }
                result.m_sources.push_back(std::move(path));
            }
            return result;
        });
    }

    Result<void> append_source(CatalogData& data, const std::string_view text, const std::string& file_path, std::size_t& definition_count) {
        return decode_document<void>(text, file_path, [&](const Json& source) {
            load_definitions(data, source, definition_count);
            return std::monostate{};
        });
    }

}
