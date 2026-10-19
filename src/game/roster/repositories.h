#ifndef PROJECTSKYLAKE_REPOSITORIES_H
#define PROJECTSKYLAKE_REPOSITORIES_H

#include "team.h"
#include "game/core/results.h"
#include "game/state/game_state.h"

class RosterRepository {
public:
    explicit RosterRepository(RosterState&);
    const CharacterInstance* find(CharacterID) const;
    std::vector<CharacterID> ids() const;
    Result<void>             insert(CharacterInstance);
    Result<void>             erase(CharacterID);

private:
    RosterState& m_state;
};

class InventoryRepository {
public:
    explicit InventoryRepository(InventoryState&);
    const ItemInstance*  find(ItemID) const;
    Result<void>         insert(ItemInstance);
    Result<ItemInstance> extract(ItemID);

private:
    InventoryState& m_state;
};

class TeamRepository {
public:
    explicit TeamRepository(TeamState&);
    const Team*  find(TeamID) const;
    Result<void> insert(Team);
    Result<void> replace(Team);
    Result<void> erase(TeamID);

private:
    TeamState& m_state;
};

#endif //PROJECTSKYLAKE_REPOSITORIES_H
