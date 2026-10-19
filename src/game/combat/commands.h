#ifndef PROJECTSKYLAKE_COMMANDS_H
#define PROJECTSKYLAKE_COMMANDS_H
#include "game/core/ids.h"
#include "game/core/types.h"

#include <variant>
#include <vector>

struct AbilityTarget {
    std::vector<GridPosition> m_tiles;
    std::vector<CombatantID>  m_combatants;
};

struct MoveCommand {
    CombatantID  m_actor;
    GridPosition m_destination;
};

struct AttackCommand {
    CombatantID m_actor;
    CombatantID m_target;
};

struct UseAbilityCommand {
    CombatantID   m_actor;
    AbilityID     m_ability;
    AbilityTarget m_target;
};

struct EndTurnCommand {
    CombatantID m_actor;
};

struct WaitCommand {
    CombatantID m_actor;
};

struct InteractCommand {
    CombatantID  m_actor;
    GridPosition m_position;
};

using BattleCommand = std::variant<MoveCommand, AttackCommand, UseAbilityCommand, EndTurnCommand, WaitCommand, InteractCommand>;

#endif //PROJECTSKYLAKE_COMMANDS_H
