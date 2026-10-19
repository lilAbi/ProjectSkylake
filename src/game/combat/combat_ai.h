#ifndef PROJECTSKYLAKE_COMBAT_AI_H
#define PROJECTSKYLAKE_COMBAT_AI_H

#include "commands.h"
#include "game/core/results.h"

class ContentCatalog;
class IRandomStream;
struct BattleState;

class ICombatAI {
public:
    virtual ~ICombatAI() = default;
    virtual Result<BattleCommand> choose_action(const BattleState&, const ContentCatalog&, IRandomStream&) const = 0;
};

#endif //PROJECTSKYLAKE_COMBAT_AI_H
