#ifndef PROJECTSKYLAKE_GAMERUNTIME_H
#define PROJECTSKYLAKE_GAMERUNTIME_H

#include "game/error/error.h"

class GameRuntime {
public:
    GameRuntime() = default;
    ~GameRuntime() = default;
    GameRuntime(const GameRuntime&) = delete;
    GameRuntime& operator = (const GameRuntime&) = delete;

public:
    Result<void>                 boot();
    Result<void>                 new_game();
    Result<void>                 load_profile();
    Result<void>                 shutdown();
    /*
    Revision                     revision() const;
    ActivityGeneration           generation() const;
    const GameState&             state() const;
    const ContentCatalog&        content() const;
    DomainEventBus&              events();
    RosterService&               roster();
    SummoningService&            summoning();
    EquipmentService&            equipment();
    CharacterProgressionService& character_progression();
    CraftingService&             crafting();
    CampService&                 camp();
    ProgressionService&          progression();
    BattleApplicationService&    battles();
    IdleProgressionService&      idle();
    SaveService&                 saves();
    ActivityCoordinator&         activities();
    */
    
private:
    //std::unique_ptr<Impl> impl_;
};

#endif //PROJECTSKYLAKE_GAMERUNTIME_H
