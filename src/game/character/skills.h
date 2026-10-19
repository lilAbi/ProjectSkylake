#ifndef PROJECTSKYLAKE_SKILLS_H
#define PROJECTSKYLAKE_SKILLS_H
#include "game/core/ids.h"
#include "game/core/results.h"

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

#endif //PROJECTSKYLAKE_SKILLS_H
