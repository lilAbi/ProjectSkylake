#ifndef PROJECTSKYLAKE_SKILL_TREE_GENERATOR_H
#define PROJECTSKYLAKE_SKILL_TREE_GENERATOR_H

#include "game/character/skills.h"
#include "game/core/results.h"

#include <span>

class ContentCatalog;
class IRandomStream;

class SkillTreeGenerator {
public:
    Result<SkillTree> generate(const ClassID& class_id, std::span<const TraitID> traits, const ContentCatalog& content_catalog, IRandomStream& skill_stream_rng) const;
};

#endif //PROJECTSKYLAKE_SKILL_TREE_GENERATOR_H
