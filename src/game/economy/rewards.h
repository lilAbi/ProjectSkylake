#ifndef PROJECTSKYLAKE_REWARDS_H
#define PROJECTSKYLAKE_REWARDS_H
#include "resources.h"
#include "game/core/ids.h"
#include "game/items/item.h"

#include <vector>

struct RewardReceipt {
    ReceiptID                           m_id;
    BattleID                            m_battle;
    ResourceBundle                      m_resources;
    std::map<CharacterID, std::int64_t> m_experience;
    std::vector<ItemID>                 m_items;
};

struct RewardBundle {
    ResourceBundle                      m_resources;
    std::map<CharacterID, std::int64_t> m_experience;
    std::vector<GeneratedItem>          m_items;
};

#endif //PROJECTSKYLAKE_REWARDS_H
