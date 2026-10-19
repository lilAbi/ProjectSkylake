#ifndef PROJECTSKYLAKE_GAME_RUNTIME_H
#define PROJECTSKYLAKE_GAME_RUNTIME_H
#include "game/core/results.h"
#include "game/core/types.h"

class ActivityCoordinator;
class BattleApplicationService;
class CampService;
class CharacterProgressionService;
class CraftingService;
class EquipmentService;
class IdleProgressionService;
class ProgressionService;
class RosterService;
class SaveService;
class SummoningService;
struct RuntimePorts;
class ContentCatalog;
class DomainEventBus;
struct GameState;
struct NewGameRequest;

class GameRuntime {
public:
    explicit GameRuntime(RuntimePorts&);
    ~GameRuntime();
    GameRuntime(const GameRuntime&) = delete;
    GameRuntime&                 operator=(const GameRuntime&) = delete;
    Result<void>                 boot();
    Result<void>                 new_game(const NewGameRequest&);
    Result<void>                 load_profile(SaveSlot);
    Result<void>                 shutdown();
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

private:
    //struct Impl;
    //std::unique_ptr<Impl> impl_;
};

using ApplicationContext = GameRuntime;

#endif //PROJECTSKYLAKE_GAME_RUNTIME_H
