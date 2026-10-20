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
    BattleApplicationService(ServiceContext& context, const EncounterGenerator& encounter_generator, const RewardGenerator& reward_generator)
        : m_context(context), m_encounter_generator(encounter_generator), m_reward_generator(reward_generator) {};
    ~BattleApplicationService() = default;
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
    const ServiceContext&     m_context;
    const EncounterGenerator& m_encounter_generator;
    const RewardGenerator&    m_reward_generator;
};

using BattleCoordinator = BattleApplicationService;

#endif //PROJECTSKYLAKE_BATTLE_APPLICATION_SERVICE_H
