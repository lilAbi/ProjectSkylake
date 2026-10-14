#ifndef PROJECTSKYLAKE_CHARACTER_CLASS_DEFINITION_H
#define PROJECTSKYLAKE_CHARACTER_CLASS_DEFINITION_H
#include "game/character/stats.h"
#include "game/id/id.h"

#include <vector>

struct CharacterClassDefinition {
    ClassID                   m_id;
    std::string               m_name;
    StatBlock                 m_base_stats;
    std::vector<AbilityID>    m_starting_abilities;
    std::vector<TraitID>      m_allowed_traits;
    std::vector<std::int64_t> m_level_thresholds;
};

#endif //PROJECTSKYLAKE_CHARACTER_CLASS_DEFINITION_H
