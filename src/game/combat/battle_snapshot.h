#ifndef PROJECTSKYLAKE_BATTLE_SNAPSHOT_H
#define PROJECTSKYLAKE_BATTLE_SNAPSHOT_H

#include "battle_state.h"
#include "game/generation/encounter_types.h"

struct BattleSnapshot {
    BattleState        m_initial_state;
    GeneratedEncounter m_encounter;
    GenerationMetadata m_generation;
};

#endif //PROJECTSKYLAKE_BATTLE_SNAPSHOT_H
