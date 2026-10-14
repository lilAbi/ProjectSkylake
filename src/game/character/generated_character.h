#ifndef PROJECTSKYLAKE_GENERATED_CHARACTER_H
#define PROJECTSKYLAKE_GENERATED_CHARACTER_H

#include "skill.h"
#include "stats.h"
#include "game/id/id.h"
#include "game/id/seed.h"

#include <vector>

struct GeneratedCharacter {
    ClassID              m_class_id;
    std::string          m_display_name;
    StatBlock            m_rolled_stats;
    std::vector<TraitID> m_traits;
    SkillTree            m_skill_tree;
    GenerationMetadata   m_generation;
};

#endif //PROJECTSKYLAKE_GENERATED_CHARACTER_H
