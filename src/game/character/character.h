#ifndef PROJECTSKYLAKE_CHARACTER_H
#define PROJECTSKYLAKE_CHARACTER_H

#include "skills.h"
#include "stats.h"
#include "game/core/ids.h"
#include "game/core/types.h"
#include "game/items/equipment.h"

#include <cstdint>
#include <vector>

struct CharacterProgression {
    std::int32_t         m_level{1};
    std::int64_t         m_experience{};
    SkillTreeProgression m_skills;
};

struct GeneratedCharacter {
    ClassID              m_class_id;
    std::string          m_display_name;
    StatBlock            m_rolled_stats;
    std::vector<TraitID> m_traits;
    SkillTree            m_skill_tree;
    GenerationMetadata   m_generation;
};

struct CharacterInstance {
    CharacterID          m_id;
    GeneratedCharacter   m_generated;
    CharacterProgression m_progression;
    Equipment            m_equipment;
};

#endif //PROJECTSKYLAKE_CHARACTER_H
