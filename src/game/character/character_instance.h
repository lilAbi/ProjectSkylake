#ifndef PROJECTSKYLAKE_CHARACTER_INSTANCE_H
#define PROJECTSKYLAKE_CHARACTER_INSTANCE_H

#include "equipment.h"
#include "generated_character.h"
#include "game/id/id.h"

struct CharacterProgression {
    std::int32_t         m_level{1};
    std::int64_t         m_experience{};
    SkillTreeProgression m_skills;
};

struct CharacterInstance {
    CharacterID          m_id;
    GeneratedCharacter   m_generated;
    CharacterProgression m_progression;
    Equipment            m_equipment;
};

#endif //PROJECTSKYLAKE_CHARACTER_INSTANCE_H
