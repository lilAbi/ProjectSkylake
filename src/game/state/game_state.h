#ifndef PROJECTSKYLAKE_GAME_STATE_H
#define PROJECTSKYLAKE_GAME_STATE_H

#include "game/character/character.h"
#include "game/core/ids.h"
#include "game/core/types.h"
#include "game/economy/rewards.h"
#include "game/items/item.h"
#include "game/roster/team.h"

#include <map>
#include <set>
#include <vector>
#include <chrono>

struct RosterState {
    std::map<CharacterID, CharacterInstance> m_characters;
};

struct InventoryState {
    std::map<ItemID, ItemInstance> m_items;
    std::int64_t                   m_capacity{100};
};

struct TeamState {
    std::map<TeamID, Team> m_teams;
};

struct BuildingInstance {
    BuildingID           m_id;
    BuildingDefinitionID m_definition;
    std::int32_t         m_level{1};
};

struct CampState {
    std::map<BuildingID, BuildingInstance> m_buildings;
};

struct ProgressionState {
    FloorNumber        m_highest_completed{0};
    RegionID           m_region;
    std::set<UnlockID> m_unlocks;
};

struct IdleAssignment {
    IdleAssignmentID         m_id;
    TeamID                   m_team;
    IdleActivityID           m_activity;
    std::chrono::sys_seconds m_checkpoint;
};

struct IdleProgressionState {
    std::vector<IdleAssignment> m_assignments;
    ResourceBundle              m_pending;
};

struct PlayerProfile {
    std::string m_profile_id;
    std::string m_display_name;
    Seed        m_master_seed;
};

struct IdentityCounters {
    std::int64_t m_next_character{1};
    std::int64_t m_next_item{1};
    std::int64_t m_next_team{1};
    std::int64_t m_next_building{1};
    std::int64_t m_next_battle{1};
    std::int64_t m_next_receipt{1};
    std::int64_t m_next_idle_assignment{1};
};

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
