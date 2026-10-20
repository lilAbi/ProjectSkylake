#ifndef PROJECTSKYLAKE_CHARACTER_PROGRESSION_SERVICE_H
#define PROJECTSKYLAKE_CHARACTER_PROGRESSION_SERVICE_H

#include "game/core/ids.h"
#include "game/core/results.h"

struct ServiceContext;

class CharacterProgressionService {
public:
    explicit CharacterProgressionService(ServiceContext& context) : m_context(context) {};
    ~CharacterProgressionService() = default;
    Result<void> unlock_skill(CharacterID, SkillNodeID);
    Result<void> level_up(CharacterID);

private:
    ServiceContext& m_context;
};

#endif //PROJECTSKYLAKE_CHARACTER_PROGRESSION_SERVICE_H
