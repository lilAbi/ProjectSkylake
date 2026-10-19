#ifndef PROJECTSKYLAKE_SKILL_TREE_GENERATOR_H
#define PROJECTSKYLAKE_SKILL_TREE_GENERATOR_H

#include "game/character/skills.h"
#include "game/core/results.h"

#include <span>

class ContentCatalog;
class IRandomStream;

class SkillTreeGenerator {
public:
    Result<SkillTree> generate(ClassID, std::span<const TraitID>, const ContentCatalog&, IRandomStream&) const;
};

#endif //PROJECTSKYLAKE_SKILL_TREE_GENERATOR_H
