#ifndef PROJECTSKYLAKE_REWARD_GENERATOR_H
#define PROJECTSKYLAKE_REWARD_GENERATOR_H

#include "game/core/results.h"
#include "game/economy/rewards.h"

class ContentCatalog;
class IRandomStream;
struct BattleResult;

class RewardGenerator {
public:
    Result<RewardBundle> generate(const BattleResult&, const ContentCatalog&, IRandomStream&) const;
};

#endif //PROJECTSKYLAKE_REWARD_GENERATOR_H
