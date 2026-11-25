#ifndef PROJECTSKYLAKE_REPOSITORIES_H
#define PROJECTSKYLAKE_REPOSITORIES_H

#include "team.h"
#include "game/core/results.h"
#include "game/state/game_state.h"

class RosterRepository {
public:
    explicit RosterRepository(RosterState& roster_state) : m_state(roster_state) {};
    const CharacterInstance* find(CharacterID character_id) const;
    std::vector<CharacterID> ids() const;
    Result<void>             insert(CharacterInstance character_instance);
    Result<void>             erase(CharacterID character_id);

private:
    RosterState& m_state;
};

class InventoryRepository {
public:
    explicit InventoryRepository(InventoryState& inventory_state) : m_state(inventory_state) {};
    const ItemInstance*  find(ItemID item_id) const;
    Result<void>         insert(ItemInstance item_instance) const;
    Result<ItemInstance> extract(ItemID item_id) const;

private:
    InventoryState& m_state;
};

class TeamRepository {
public:
    explicit TeamRepository(TeamState& team_state) : m_state(team_state) {};
    const Team*  find(TeamID team_id) const;
    Result<void> insert(Team team);
    Result<void> replace(Team team);
    Result<void> erase(TeamID item_id);

private:
    TeamState& m_state;
};

#endif //PROJECTSKYLAKE_REPOSITORIES_H
