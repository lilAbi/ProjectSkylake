#ifndef PROJECTSKYLAKE_GAME_STATE_H
#define PROJECTSKYLAKE_GAME_STATE_H

#include "camp_state.h"
#include "idle_progression_state.h"
#include "identity_counters.h"
#include "progression_state.h"
#include "inventory_state.h"
#include "player_profile.h"
#include "resource_wallet.h"
#include "reward_receipt.h"
#include "roster_state.h"
#include "team_state.h"

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
    std::map<BattleID, RewardReceipt> m_settled_battles;
};

#endif //PROJECTSKYLAKE_GAME_STATE_H
