#ifndef PROJECTSKYLAKE_RULES_H
#define PROJECTSKYLAKE_RULES_H

#include "battle_results.h"
#include "commands.h"
#include "events.h"
#include "game/content/definitions.h"
#include "game/core/ids.h"
#include "game/core/results.h"
#include "game/core/types.h"

#include <span>
#include <vector>

class ContentCatalog;
class IRandomStream;
struct AbilityDefinition;
struct BattleState;

class BattleRules {
public:
    Result<void> validate(const BattleState&, const BattleCommand&, const ContentCatalog&) const;
};

class MovementRules {
public:
    Result<std::vector<GridPosition>> path(const BattleState&, CombatantID, GridPosition) const;
    Result<std::vector<GridPosition>> reachable(const BattleState&, CombatantID) const;
};

class TargetingRules {
public:
    Result<AbilityTarget> resolve(const BattleState&, CombatantID, const AbilityDefinition&, const AbilityTarget&) const;
};

class EffectResolver {
public:
    Result<std::vector<BattleEvent>> apply(BattleState& candidate, CombatantID source, const AbilityTarget&, std::span<const EffectDefinition>, const ContentCatalog&, IRandomStream&) const;
};

class StatusSystem {
public:
    Result<std::vector<BattleEvent>> begin_turn(BattleState& candidate, CombatantID, const ContentCatalog&, IRandomStream&) const;
    Result<std::vector<BattleEvent>> end_turn(BattleState& candidate, CombatantID, const ContentCatalog&) const;
};

class ObjectiveEvaluator {
public:
    std::optional<Battle_outcome> evaluate(const BattleState&, const ContentCatalog&) const;
};

#endif //PROJECTSKYLAKE_RULES_H
