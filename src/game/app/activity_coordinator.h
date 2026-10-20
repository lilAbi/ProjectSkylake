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
    ActivityCoordinator(ProgressionService& progression_service, RosterService& roster_service, BattleApplicationService& battle_application_service)
        : m_progression_service(progression_service), m_roster_service(roster_service), m_battle_application_service(battle_application_service) {};
    Result<NavigationPlan> plan_camp(Revision) const;
    Result<NavigationPlan> plan_battle(const StartBattleRequest&, Revision) const;

private:
    ProgressionService&       m_progression_service;
    RosterService&            m_roster_service;
    BattleApplicationService& m_battle_application_service;
};

#endif //PROJECTSKYLAKE_ACTIVITY_COORDINATOR_H
