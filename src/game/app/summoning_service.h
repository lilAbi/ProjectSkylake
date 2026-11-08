#ifndef PROJECTSKYLAKE_SUMMONING_SERVICE_H
#define PROJECTSKYLAKE_SUMMONING_SERVICE_H

#include "requests.h"
#include "views.h"

struct ServiceContext;
class CharacterGenerator;

constexpr std::int32_t  maximum_summons_per_request = 10;
constexpr std::uint32_t generator_version = 1;

class SummoningService {
public:
    SummoningService(ServiceContext& context, const CharacterGenerator& character_generator) : m_context(context), m_character_generator(character_generator) {};
    ~SummoningService() = default;

    std::vector<BannerView> get_banners();
    Result<SummonResult>    summon(const SummonRequest& summon_request);

private:
    Result<ResourceCost> calculate_total_banner_cost(
        const BannerDefinition& banner_definition,
        std::int32_t            count,
        const ContentCatalog&   content_catalog
    );

    bool is_banner_unlocked(const BannerDefinition& banner_definition, const GameState& game_state);

private:
    ServiceContext&           m_context;
    const CharacterGenerator& m_character_generator;
};

#endif //PROJECTSKYLAKE_SUMMONING_SERVICE_H
