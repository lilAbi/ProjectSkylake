#ifndef PROJECTSKYLAKE_SAVE_TYPES_H
#define PROJECTSKYLAKE_SAVE_TYPES_H

#include "game/core/types.h"
#include "game/state/game_state.h"
#include "game/core/results.h"

#include <vector>
#include <chrono>
#include <map>

struct SaveHeader {
    std::uint32_t m_format_version{1};
    std::uint32_t m_schema_version{1};
    std::string   m_content_version;
    std::string   m_codec;
};

struct SaveEnvelope {
    SaveHeader             m_header;
    std::vector<std::byte> m_payload;
};

struct SaveMetadata {
    SaveSlot                 m_slot;
    std::chrono::sys_seconds m_saved_at;
    std::string              m_profile_id;
    FloorNumber              m_highest_completed;
};

struct SaveSlotInfo {
    SaveSlot                    m_slot;
    std::optional<SaveMetadata> m_metadata;
    std::optional<Error>        m_error;
};

struct SaveDocument {
    std::uint32_t                      m_schema_version{1};
    std::map<std::string, std::string> m_canonical_sections;
};

struct LoadCandidate {
    GameState    m_state;
    SaveMetadata m_metadata;
    std::string  m_content_version;
};

#endif //PROJECTSKYLAKE_SAVE_TYPES_H
