#ifndef PROJECTSKYLAKE_BATTLE_RUNNER_H
#define PROJECTSKYLAKE_BATTLE_RUNNER_H

#include "battle_session.h"
#include "game/core/results.h"

#include <cstdint>

class ContentCatalog;
class ICombatAI;

struct SimulationBudget {
    std::int32_t m_maximum_turns{1000};
    std::int32_t m_maximum_commands{10000};
};

class BattleRunner {
public:
    Result<BattleResult> run(BattleSession&, const ContentCatalog&, const ICombatAI&, IRandomStream&, SimulationBudget) const;
};

#endif //PROJECTSKYLAKE_BATTLE_RUNNER_H
