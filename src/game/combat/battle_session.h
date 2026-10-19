#ifndef PROJECTSKYLAKE_BATTLE_SESSION_H
#define PROJECTSKYLAKE_BATTLE_SESSION_H

#include "battle_results.h"
#include "battle_snapshot.h"
#include "commands.h"
#include "game/core/random.h"
#include "game/core/results.h"

#include <memory>

class ContentCatalog;

class BattleSession {
public:
    static Result<std::unique_ptr<BattleSession>> create(BattleSnapshot, const ContentCatalog&, std::unique_ptr<IRandomStream>);
    ~BattleSession();
    const BattleState&          state() const;
    Result<BattleCommandResult> execute(const BattleCommand&, Revision expected);
    bool                        finished() const;
    std::optional<BattleResult> result() const;

private:
    BattleSession();
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif //PROJECTSKYLAKE_BATTLE_SESSION_H
