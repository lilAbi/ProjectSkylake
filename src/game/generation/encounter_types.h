#ifndef PROJECTSKYLAKE_ENCOUNTER_TYPES_H
#define PROJECTSKYLAKE_ENCOUNTER_TYPES_H

#include "game/character/stats.h"
#include "game/content/definitions.h"
#include "game/core/ids.h"
#include "game/core/types.h"

#include <vector>

struct EncounterGenerationRequest {
    FloorNumber        m_floor;
    RegionID           m_region;
    std::int32_t       m_tier{1};
    GenerationMetadata m_metadata;
};

struct EnemySpawn {
    EnemyID      m_definition;
    GridPosition m_position;
    StatBlock    m_stats;
};

struct GeneratedEncounter {
    EncounterDefinition     m_definition;
    FloorNumber             m_floor;
    std::vector<EnemySpawn> m_enemies;
    GenerationMetadata      m_generation;
};

#endif //PROJECTSKYLAKE_ENCOUNTER_TYPES_H
