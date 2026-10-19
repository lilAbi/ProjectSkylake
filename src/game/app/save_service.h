#ifndef PROJECTSKYLAKE_SAVE_SERVICE_H
#define PROJECTSKYLAKE_SAVE_SERVICE_H

#include "game/core/results.h"
#include "game/persistence/save_types.h"

class ContentCatalog;
class IClock;
struct GameState;
class ISaveCodec;
class ISaveStore;
class MigrationRegistry;
class SaveMapper;

class SaveService {
public:
    SaveService(ISaveStore&, const ISaveCodec&, const MigrationRegistry&, const SaveMapper&, const ContentCatalog&, const IClock&);
    ~SaveService();
    Result<void>                      save(SaveSlot, const GameState&);
    Result<LoadCandidate>             load_candidate(SaveSlot) const;
    Result<std::vector<SaveSlotInfo>> slots() const;
    Result<void>                      erase(SaveSlot);
};

#endif //PROJECTSKYLAKE_SAVE_SERVICE_H
