#ifndef PROJECTSKYLAKE_BATTLE_RESULTS_H
#define PROJECTSKYLAKE_BATTLE_RESULTS_H

#include "events.h"
#include "game/core/ids.h"
#include "game/core/types.h"

#include <optional>
#include <vector>

enum class Battle_outcome {
    victory,
    defeat,
    surrender,
    turn_limit
};

struct BattleResult {
    BattleID                 m_battle;
    Battle_outcome           m_outcome;
    FloorNumber              m_floor;
    RewardTableID            m_rewards;
    std::vector<CharacterID> m_participants;
    std::int32_t             m_turns{};
};

struct BattleCommandResult {
    BattleID                    m_battle;
    Revision                    m_revision;
    std::vector<BattleEvent>    m_events;
    std::optional<BattleResult> m_finished;
};

#endif //PROJECTSKYLAKE_RESULTS_H
