#ifndef PROJECTSKYLAKE_BATTLE_STATE_H
#define PROJECTSKYLAKE_BATTLE_STATE_H

#include "grid.h"
#include "game/character/stats.h"
#include "game/core/ids.h"
#include "game/core/types.h"

#include <map>
#include <optional>
#include <vector>

struct StatusInstance {
    StatusID     m_definition;
    CombatantID  m_source;
    std::int32_t m_remaining_turns{1};
    std::int32_t m_stacks{1};
};

struct Combatant {
    CombatantID                       m_id;
    std::optional<CharacterID>        m_persistent_character;
    std::int32_t                      m_faction{};
    GridPosition                      m_position;
    StatBlock                         m_stats;
    std::int64_t                      m_hp{};
    std::int32_t                      m_action_points{};
    std::int32_t                      m_movement_points{};
    std::vector<AbilityID>            m_abilities;
    std::map<AbilityID, std::int32_t> m_cooldowns;
    std::vector<StatusInstance>       m_statuses;
};

enum class Battle_phase {
    deployment,
    awaiting_command,
    finished
};

struct ObjectiveState {
    ObjectiveID  m_definition;
    std::int32_t m_progress{};
    bool         m_completed{};
};

struct BattleState {
    BattleID                    m_id;
    Revision                    m_revision;
    Battle_phase                m_phase{Battle_phase::deployment};
    Grid                        m_grid;
    std::vector<Combatant>      m_combatants;
    std::optional<CombatantID>  m_active_actor;
    std::int32_t                m_turn{};
    std::vector<ObjectiveState> m_objectives;
};

#endif //PROJECTSKYLAKE_BATTLE_STATE_H
