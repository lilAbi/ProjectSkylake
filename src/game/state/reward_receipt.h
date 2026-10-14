#ifndef PROJECTSKYLAKE_REWARD_RECEIPT_H
#define PROJECTSKYLAKE_REWARD_RECEIPT_H

#include "game/id/id.h"
#include "game/id/resource_bundle.h"

#include <vector>

struct RewardReceipt {
    ReceiptID                           m_id;
    BattleID                            m_battle;
    ResourceBundle                      m_resources;
    std::map<CharacterID, std::int64_t> m_experience;
    std::vector<ItemID>                 m_items;
};

#endif //PROJECTSKYLAKE_REWARD_RECEIPT_H
