#ifndef PROJECTSKYLAKE_BATTLE_APPLICATION_SERVICE_H
#define PROJECTSKYLAKE_BATTLE_APPLICATION_SERVICE_H

#include "requests.h"
#include "game/combat/battle_state.h"
#include "game/combat/commands.h"
#include "game/core/ids.h"
#include "game/core/results.h"
#include "game/economy/rewards.h"

#include <memory>

struct ServiceContext;
class EncounterGenerator;
class RewardGenerator;
struct StartBattleRequest;

class BattleApplicationService {
public:
    BattleApplicationService(ServiceContext&, const EncounterGenerator&, const RewardGenerator&);
    ~BattleApplicationService();
    Result<BattleStart>               start(const StartBattleRequest&);
    Result<BattleState>               snapshot(BattleID) const;
    Result<std::vector<GridPosition>> reachable(BattleID, CombatantID) const;
    Result<AbilityTarget>             preview_ability(BattleID, CombatantID, const AbilityID&, const AbilityTarget&) const;
    Result<BattleExecution>           execute(BattleID, const BattleCommand&, Revision expected);
    Result<RewardReceipt>             settle(BattleID);
    Result<void>                      surrender(BattleID);
    Result<void>                      cancel(BattleID);
    std::optional<BattleID>           active_battle() const;

private:
    //struct Impl;
    //std::unique_ptr<Impl> impl_;
};

using BattleCoordinator = BattleApplicationService;

#endif //PROJECTSKYLAKE_BATTLE_APPLICATION_SERVICE_H
