#ifndef PROJECTSKYLAKE_REQUESTS_H
#define PROJECTSKYLAKE_REQUESTS_H

#include "game/combat/battle_results.h"
#include "game/combat/battle_snapshot.h"
#include "game/core/ids.h"
#include "game/core/types.h"
#include "game/economy/resources.h"
#include "game/economy/rewards.h"

#include <vector>

struct SummonRequest {
    BannerID     m_banner;
    std::int32_t m_count{1};
};

struct SummonResult {
    std::vector<CharacterID> m_characters;
    ResourceCost             m_spent;
    Revision                 m_revision;
};

struct CraftRequest {
    RecipeID     m_recipe;
    std::int32_t m_quantity{1};
};

struct CraftResult {
    std::vector<ItemID> m_items;
    ResourceCost        m_spent;
    Revision            m_revision;
};

struct StartBattleRequest {
    TeamID      m_team;
    FloorNumber m_floor;
    RegionID    m_region;
};

struct BattleStart {
    BattleID       m_battle;
    BattleSnapshot m_snapshot;
};

struct BattleExecution {
    BattleCommandResult          m_command;
    std::optional<RewardReceipt> m_settlement;
};

struct NewGameRequest {
    std::string m_profile_id;
    std::string m_display_name;
    Seed        m_master_seed;
};

#endif //PROJECTSKYLAKE_REQUESTS_H
