#ifndef PROJECTSKYLAKE_JSON_HELPER_H
#define PROJECTSKYLAKE_JSON_HELPER_H

#include <cstddef>
//#include <nlohmann/json.hpp>

//using Json = nlohmann::json;
constexpr std::size_t max_file_bytes = 16 * 1024 * 1024;
constexpr std::size_t max_total_bytes = 64 * 1024 * 1024;
constexpr std::size_t max_sources = 256;
constexpr std::size_t max_definitions = 100'000;
constexpr int         max_json_depth = 64;

#endif //PROJECTSKYLAKE_JSON_HELPER_H
