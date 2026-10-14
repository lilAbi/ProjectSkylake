#ifndef PROJECTSKYLAKE_GAME_STATE_H
#define PROJECTSKYLAKE_GAME_STATE_H
#include <map>

struct GameState {
    PlayerProfile                     m_profile;
    RosterState                       m_roster;
    InventoryState                    m_inventory;
    TeamState                         m_teams;
    ResourceWallet                    m_wallet;
    CampState                         m_camp;
    ProgressionState                  m_progression;
    IdleProgressionState              m_idle;
    IdentityCounters                  m_identities;
    std::map<BattleId, RewardReceipt> m_settled_battles;
};

#endif //PROJECTSKYLAKE_GAME_STATE_H
