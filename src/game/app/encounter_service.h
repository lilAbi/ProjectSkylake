#ifndef PROJECTSKYLAKE_ENCOUNTER_SERVICE_H
#define PROJECTSKYLAKE_ENCOUNTER_SERVICE_H

#include "game/core/results.h"
#include "game/generation/encounter_types.h"

struct ServiceContext;
class EncounterGenerator;
struct StartBattleRequest;

class EncounterService {
public:
    EncounterService(ServiceContext&, const EncounterGenerator&);
    Result<GeneratedEncounter> prepare(const StartBattleRequest&, BattleID) const;
private:
    ServiceContext& context_; const EncounterGenerator& generator_;
};

#endif //PROJECTSKYLAKE_ENCOUNTER_SERVICE_H
