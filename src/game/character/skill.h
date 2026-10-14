#ifndef PROJECTSKYLAKE_SKILL_H
#define PROJECTSKYLAKE_SKILL_H

#include "game/error/error.h"
#include "game/id/id.h"

#include <set>
#include <vector>

struct SkillNode {
    SkillNodeID              m_id;
    AbilityID                m_ability;
    std::vector<SkillNodeID> m_prerequisites;
    std::int32_t             m_point_cost{1};
};

struct SkillTree {
    std::vector<SkillNode> m_nodes;
    Result<void>           validate() const;
};

struct SkillTreeProgression {
    std::set<SkillNodeID> m_unlocked;
    std::int32_t          m_available_points{};
};


#endif //PROJECTSKYLAKE_SKILL_H
