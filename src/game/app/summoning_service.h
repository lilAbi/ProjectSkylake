#ifndef PROJECTSKYLAKE_SUMMONING_SERVICE_H
#define PROJECTSKYLAKE_SUMMONING_SERVICE_H

#include "requests.h"
#include "views.h"

struct ServiceContext;
class CharacterGenerator;

class SummoningService {
public:
    SummoningService(ServiceContext&, const CharacterGenerator&);
    std::vector<BannerView> banners() const;
    Result<SummonResult>    summon(const SummonRequest&);

private:
    ServiceContext&           m_context;
    const CharacterGenerator& m_character_generator;
};

#endif //PROJECTSKYLAKE_SUMMONING_SERVICE_H
