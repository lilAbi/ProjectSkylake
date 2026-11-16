#include "json_content_source.h"

#include "json_decoding.h"

#include <godot_cpp/classes/file_access.hpp>

#include <cstdint>

namespace {

    Result<std::string> read_godot_file(const std::string& path, const std::size_t max_bytes) {
        const auto file = godot::FileAccess::open(godot::String::utf8(path.c_str()), godot::FileAccess::READ);

        if (file.is_null()) {
            return Error{
                .m_code = Error_code::io_failure,
                .m_message = path + ": cannot open content file (Godot error " + std::to_string(godot::FileAccess::get_open_error()) + ")"
            };
        }

        const auto length = file->get_length();
        if (length > max_bytes) {
            return Error{
                .m_code = Error_code::invalid_content,
                .m_message = path + ": content byte limit exceeded"
            };
        }

        // max_bytes is at most 16 MiB, so the conversion to Godot's signed size is safe.
        const auto bytes = file->get_buffer(static_cast<std::int64_t>(length));
        if (static_cast<std::uint64_t>(bytes.size()) != length) {
            return Error{
                .m_code = Error_code::io_failure,
                .m_message = path + ": short read while loading content"
            };
        }
        return bytes.is_empty() ? std::string{} : std::string{reinterpret_cast<const char*>(bytes.ptr()), static_cast<std::size_t>(bytes.size())};
    }

}

JSONContentSource::JSONContentSource(std::string manifest_path) : JSONContentSource(std::move(manifest_path), read_godot_file) {}

Result<std::string> JSONContentSource::read_file(const std::string& path) {
    return read_godot_file(path, content::json_detail::max_file_bytes);
}
