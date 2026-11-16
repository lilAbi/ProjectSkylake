#ifndef PROJECTSKYLAKE_JSON_READER_H
#define PROJECTSKYLAKE_JSON_READER_H

#include "game/core/ids.h"
#include "game/core/results.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

// Implementation only: JSON conversion and its contextual exceptions stay here.
namespace content::json_detail {

    using Json = nlohmann::json;

    [[noreturn]] inline void fail(const std::string& path, const std::string& message, Error_code code = Error_code::invalid_content) {
        throw Error{
            .m_code = code,
            .m_message = path + ": " + message
        };
    }

    inline std::string child_path(const std::string& path, const std::string_view key) {
        std::string result = path + "/";
        for (const char c : key) {
            result += c == '~' ? "~0" : c == '/' ? "~1"
                                                 : std::string(1, c);
        }
        return result;
    }

    template <typename T>
    struct VectorType : std::false_type {};
    template <typename T>
    struct VectorType<std::vector<T>> : std::true_type {
        using Element = T;
    };

    template <typename T>
    struct DefinitionIDType : std::false_type {};
    template <typename Tag>
    struct DefinitionIDType<DefinitionID<Tag>> : std::true_type {};

    template <typename T>
    struct EnumNames;
    template <typename T>
    struct Decoder;

    template <typename T>
    T decode(const Json& value, const std::string& path) {
        //attempt to extract a string if T is a string
        if constexpr (std::is_same_v<T, std::string>) {
            if (!value.is_string()) {
                fail(path, "expected a string");
            }
            auto result = value.get<std::string>();
            if (result.find('\0') != std::string::npos) {
                fail(path, "embedded NUL is not allowed");
            }
            return result;
        } else if constexpr (std::is_same_v<T, bool>) {
            //attempt to extract a boolean if T is a bool
            if (!value.is_boolean()) {
                fail(path, "expected a boolean");
            }
            return value.get<bool>();
        } else if constexpr (std::integral<T>) {
            //attempt to extract an unsigned int if T is an integer type
            if (value.is_number_unsigned()) {
                const auto number = value.get<std::uint64_t>();
                if (!std::in_range<T>(number)) {
                    fail(path, "integer is outside the destination type's range");
                }
                return static_cast<T>(number);
            }
            //attempt to extract an int if T is an integer type
            if (value.is_number_integer()) {
                const auto number = value.get<std::int64_t>();
                if (!std::in_range<T>(number)) {
                    fail(path, "integer is outside the destination type's range");
                }
                return static_cast<T>(number);
            }
            fail(path, "expected an integer (floating-point values are not accepted)");
        } else if constexpr (DefinitionIDType<T>::value) {
            //attempt to extract an DefinitionID value if T is an DefinitionID type
            return T{decode<std::string>(value, path)};
        } else if constexpr (std::is_enum_v<T>) {
            //attempt to extract an enum value
            const auto name = decode<std::string>(value, path);
            for (const auto& [label, result] : EnumNames<T>::values) {
                if (name == label) {
                    return result;
                }
            }
            fail(path, "unknown enum value '" + name + "'");
        } else if constexpr (VectorType<T>::value) {
            if (!value.is_array()) {
                fail(path, "expected an array");
            }
            T result;
            result.reserve(value.size());
            for (std::size_t index = 0; index < value.size(); ++index) {
                result.push_back(decode<typename VectorType<T>::Element>(value[index], child_path(path, std::to_string(index))));
            }
            return result;
        } else {
            return Decoder<T>::read(value, path);
        }
    }

    class ObjectReader {
    public:
        ObjectReader(const Json& value, std::string path, const std::initializer_list<std::string_view> consumed = {})
            : m_value(value), m_path(std::move(path)) {
            if (!m_value.is_object()) {
                fail(m_path, "expected an object");
            }
            for (const auto name : consumed) {
                m_fields.emplace_back(name);
            }
        }

        template <typename T>
        void required(std::string_view name, T& destination) {
            field(name, destination, true);
        }

        template <typename T>
        void optional(std::string_view name, T& destination) {
            field(name, destination, false);
        }

        // Call after declaring all fields, including optional fields absent from the JSON.
        void finish() const {
            for (const auto& [key, ignored] : m_value.items()) {
                if (std::ranges::find(m_fields, key) == m_fields.end()) {
                    fail(child_path(m_path, key), "unknown field");
                }
            }
        }

    private:
        template <typename T>
        void field(std::string_view name, T& destination, const bool required) {
            m_fields.emplace_back(name);
            const auto it = m_value.find(std::string{name});
            if (it == m_value.end()) {
                if (required) {
                    fail(child_path(m_path, name), "missing required field");
                }
                return;
            }
            destination = decode<T>(*it, child_path(m_path, name));
        }

        const Json&              m_value;
        std::string              m_path;
        std::vector<std::string> m_fields;
    };

}

#endif //PROJECTSKYLAKE_JSON_READER_H
