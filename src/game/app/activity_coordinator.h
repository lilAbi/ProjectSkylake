#ifndef PROJECTSKYLAKE_ACTIVITY_COORDINATOR_H
#define PROJECTSKYLAKE_ACTIVITY_COORDINATOR_H

#include "requests.h"
#include "game/core/results.h"
#include "game/core/types.h"

#include <optional>

struct Revision;
class BattleApplicationService;
class ProgressionService;
class RosterService;

enum class Activity_kind {
    camp,
    battle
};

enum class Screen_kind {
    roster,
    summon,
    crafting
};

struct NavigationPlan {
    Activity_kind                     m_activity;
    std::optional<StartBattleRequest> m_battle_request;
    Revision                          m_expected_revision;
};

class ActivityCoordinator {
public:
    ActivityCoordinator(ProgressionService&, RosterService&, BattleApplicationService&);
    Result<NavigationPlan> plan_camp(Revision) const;
    Result<NavigationPlan> plan_battle(const StartBattleRequest&, Revision) const;

private:
    ProgressionService&       m_progression;
    RosterService&            m_roster;
    BattleApplicationService& m_battles;
};

#endif //PROJECTSKYLAKE_ACTIVITY_COORDINATOR_H
