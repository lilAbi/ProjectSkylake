#ifndef PROJECTSKYLAKE_CHARACTER_GENERATION_H
#define PROJECTSKYLAKE_CHARACTER_GENERATION_H

#include "game/character/character.h"
#include "game/core/ids.h"
#include "game/core/results.h"
#include "game/core/types.h"

#include <vector>

class ContentCatalog;
class IRandomStream;

struct CharacterGenerationRequest {
    std::vector<ClassID> m_allowed_classes;
    std::int32_t         m_tier{1};
    GenerationMetadata   m_metadata;
};

class CharacterGenerator {
public:
    explicit CharacterGenerator(const ContentCatalog& content_catalog) : m_content_catalog(content_catalog) {};
    ~CharacterGenerator() = default;
    Result<GeneratedCharacter> generate(const CharacterGenerationRequest&, IRandomStream& character_rng, IRandomStream& skill_tree_rng) const;

private:
    const ContentCatalog& m_content_catalog;
};

#endif //PROJECTSKYLAKE_CHARACTER_GENERATION_H
