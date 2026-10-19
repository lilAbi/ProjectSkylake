#ifndef PROJECTSKYLAKE_SAVE_MAPPER_H
#define PROJECTSKYLAKE_SAVE_MAPPER_H

#include "dto.h"
#include "game/content/catalog.h"
#include "game/core/results.h"
#include "game/state/game_state.h"

class SaveMapper {
public:
    Result<SaveGameDTO> to_dto(const GameState&, const ContentCatalog&) const;
    Result<GameState>   from_dto(const SaveGameDTO&, const ContentCatalog&) const;
};

#endif //PROJECTSKYLAKE_SAVE_MAPPER_H
