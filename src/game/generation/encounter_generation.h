#ifndef PROJECTSKYLAKE_ENCOUNTER_GENERATION_H
#define PROJECTSKYLAKE_ENCOUNTER_GENERATION_H
#include "game/core/results.h"

class ContentCatalog;
class IRandomStream;
struct EncounterGenerationRequest;

class EncounterGenerator {
public:
    explicit EncounterGenerator(const ContentCatalog&);
    Result<GeneratedEncounter> generate(const EncounterGenerationRequest&, IRandomStream&) const;

private:
    const ContentCatalog& m_content_catalog;
};

#endif //PROJECTSKYLAKE_ENCOUNTER_GENERATION_H
