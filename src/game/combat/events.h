#ifndef PROJECTSKYLAKE_EVENTS_H
#define PROJECTSKYLAKE_EVENTS_H

#include "battle_state.h"
#include "commands.h"
#include "game/core/ids.h"
#include "game/core/types.h"

#include <variant>
#include <vector>

struct ActorMoved {
    CombatantID               m_actor;
    std::vector<GridPosition> m_path;
};

struct AbilityStarted {
    CombatantID   m_actor;
    AbilityID     m_ability;
    AbilityTarget m_target;
};

struct DamageApplied {
    CombatantID  m_source;
    CombatantID  m_target;
    std::int64_t m_amount{};
    std::int64_t m_old_hp{};
    std::int64_t m_new_hp{};
};

struct HealingApplied {
    CombatantID  m_target;
    std::int64_t m_amount{};
    std::int64_t m_old_hp{};
    std::int64_t m_new_hp{};
};

struct StatusApplied {
    CombatantID    m_target;
    StatusInstance m_status;
};

struct CombatantKilled {
    CombatantID m_actor;
};

struct TurnEnded {
    CombatantID  m_actor;
    std::int32_t m_turn{};
};

using BattleEventPayload = std::variant<ActorMoved, AbilityStarted, DamageApplied, HealingApplied, StatusApplied, CombatantKilled, TurnEnded>;

struct BattleEvent {
    std::int64_t       m_sequence{};
    BattleEventPayload m_payload;
};

#endif //PROJECTSKYLAKE_EVENTS_H
