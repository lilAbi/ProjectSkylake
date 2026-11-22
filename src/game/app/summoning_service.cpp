#include "summoning_service.h"

#include "game/content/catalog.h"
#include "game/core/random.h"
#include "game/generation/character_generation.h"
#include "game/roster/repositories.h"

std::vector<BannerView> SummoningService::get_banners() {
    std::vector<BannerView> banner_views;
    const auto              banner_definitions = m_context.m_content_catalog.banners();
    banner_views.reserve(banner_definitions.size());
    //iterate through each banner and grab its metadata
    for (const auto& banner_definition : banner_definitions) {
        const auto  total_summon_cost_result = this->calculate_total_banner_cost(banner_definition, 1, m_context.m_content_catalog);
        const auto* total_summon_cost = std::get_if<ResourceCost>(&total_summon_cost_result);
        banner_views.push_back(
            BannerView{
                .m_id = banner_definition.m_id,
                .m_cost_per_summon = banner_definition.m_cost_per_summon,
                .m_affordable = total_summon_cost && m_context.m_game_state.m_wallet.can_afford(*total_summon_cost),
                .m_unlocked = this->is_banner_unlocked(banner_definition, m_context.m_game_state)
            }
        );
    }
    return banner_views;
}

Result<SummonResult> SummoningService::summon(const SummonRequest& summon_request) {
    constexpr auto maximum = std::numeric_limits<std::int64_t>::max();
    //check if executing via EventBus
    const auto result = check_service_mutation(m_context);
    if (const auto* error = std::get_if<Error>(&result); error) {
        return *error;
    }
    //check is the request summon count is within allowed limits
    if (summon_request.m_count < 1 || summon_request.m_count > maximum_summons_per_request) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Summon count must be between 1 and 10."
        };
    }
    auto&       game_state = m_context.m_game_state;
    const auto& content_catalog = m_context.m_content_catalog;
    //grab the requested banner definition
    const auto* banner_definition = content_catalog.find(summon_request.m_banner);
    //check if banner exist
    if (!banner_definition) {
        return Error{
            .m_code = Error_code::not_found,
            .m_message = "The requested banner is not found."
        };
    }
    //check if banner is unlocked
    if (!this->is_banner_unlocked(*banner_definition, game_state)) {
        return Error{
            .m_code = Error_code::locked,
            .m_message = "This banner is locked."
        };
    }
    //check if banner has any classes to summon from
    if (banner_definition->m_allowed_classes.empty()) {
        return Error{
            .m_code = Error_code::invalid_content,
            .m_message = "Summoning requires classes."
        };
    }
    //check if the classes in the banner actually exist
    for (const auto& class_id : banner_definition->m_allowed_classes) {
        if (!content_catalog.find(class_id)) {
            return Error{
                .m_code = Error_code::invalid_content,
                .m_message = "A banner references a missing class."
            };
        }
    }
    //grab total cost of the requested amount of summons to perform
    const auto total_summon_cost_result = this->calculate_total_banner_cost(*banner_definition, summon_request.m_count, content_catalog);
    if (const auto* error = std::get_if<Error>(&total_summon_cost_result)) {
        return *error;
    }
    const auto* total_summon_cost = std::get_if<ResourceCost>(&total_summon_cost_result);
    //check if the player has enough to summon in their wallet
    if (!game_state.m_wallet.can_afford(*total_summon_cost)) {
        return Error{
            .m_code = Error_code::insufficient_resources,
            .m_message = "Not enough resources to summon."
        };
    }
    //grab next id counter
    const auto next_character_id = game_state.m_identities.m_next_character;
    //check if new character id
    if (next_character_id < 1 || m_context.m_revision.m_value < 0) {
        return Error{
            .m_code = Error_code::invalid_phase,
            .m_message = "Character identities or state revision is invalid."
        };
    }
    //make sure the next id is valid
    if (next_character_id > maximum - summon_request.m_count || m_context.m_revision.m_value == maximum) {
        return Error{
            .m_code = Error_code::overflow,
            .m_message = "Character identities or state revision would overflow."
        };
    }
    //check if the next character ids are in use
    for (std::int32_t i = 0; i < summon_request.m_count; ++i) {
        if (game_state.m_roster.m_characters.contains(CharacterID{next_character_id + i})) {
            return Error{
                .m_code = Error_code::invalid_phase,
                .m_message = "The next character identity is already in use."
            };
        }
    }
    //make a copy of the current state
    auto wallet_state_copy = game_state.m_wallet;
    //attempt to spend and check for errors
    auto spend_result = wallet_state_copy.try_spend(*total_summon_cost);
    if (const auto* error = std::get_if<Error>(&spend_result)) {
        return *error;
    }
    //make a copy of the roster
    auto                     roster_state_copy = game_state.m_roster;
    std::vector<CharacterID> summoned_character_ids;
    summoned_character_ids.reserve(static_cast<std::size_t>(summon_request.m_count));
    //create a request to generate summonds
    CharacterGenerationRequest request{
        .m_allowed_classes = banner_definition->m_allowed_classes,
        .m_tier = 1,
        .m_metadata = GenerationMetadata{
            .m_seed = game_state.m_profile.m_master_seed,
            .m_generator_version = generator_version,
            .m_content_version = content_catalog.content_version()
        }
    };
    //perform the summons
    for (std::int32_t i = 0; i < summon_request.m_count; ++i) {
        //create character id
        const CharacterID new_character_id{next_character_id + 1};
        //create a rng stream for this summon and checks for errors
        auto character_rng_stream_result = m_context.m_rng.make_stream(
            game_state.m_profile.m_master_seed,
            StreamKey{
                .m_domain = "summon.character",
                .m_instance = static_cast<std::uint64_t>(new_character_id.m_value),
                .m_algorithm_version = generator_version
            }
        );
        if (const auto* error = std::get_if<Error>(&character_rng_stream_result)) {
            return *error;
        }
        //create a rng stream for the summon's skills and checks for errors
        auto skills_rng_stream_result = m_context.m_rng.make_stream(
            game_state.m_profile.m_master_seed,
            StreamKey{
                .m_domain = "summon.skill_tree",
                .m_instance = static_cast<std::uint64_t>(new_character_id.m_value),
                .m_algorithm_version = generator_version
            }
        );
        if (const auto* error = std::get_if<Error>(&skills_rng_stream_result)) {
            return *error;
        }
        //grab the actual stream objs and check for errors
        auto& character_rng_stream = std::get<std::unique_ptr<IRandomStream>>(character_rng_stream_result);
        auto& skill_rng_stream = std::get<std::unique_ptr<IRandomStream>>(skills_rng_stream_result);
        if (!character_rng_stream || !skill_rng_stream) {
            return Error{
                .m_code = Error_code::not_configured,
                .m_message = "The random stream factory returned an empty stream."
            };
        }
        // generate summon
        auto summoned_resulted = m_character_generator.generate(request, *character_rng_stream, *skill_rng_stream);
        if (const auto* error = std::get_if<Error>(&summoned_resulted)) {
            return *error;
        }
        auto& summoned_character = std::get<GeneratedCharacter>(summoned_resulted);
        // test if summoned character is allowed with respect to the current banner
        if (std::ranges::find(banner_definition->m_allowed_classes, summoned_character.m_class_id) == banner_definition->m_allowed_classes.end()) {
            return Error{
                .m_code = Error_code::invalid_content,
                .m_message = "The generator returned a class outside this banner."
            };
        }
        //create character instance
        CharacterInstance character_instance{
            .m_id = new_character_id,
            .m_generated = std::move(summoned_character),
            .m_progression = {},
            .m_equipment = {}
        };
        // unlock nodes
        for (const auto& node : character_instance.m_generated.m_skill_tree.m_nodes) {
            if (node.m_point_cost == 0 && node.m_prerequisites.empty()) {
                character_instance.m_progression.m_skills.m_unlocked.insert(node.m_id);
            }
        }
        //
        auto inserted = RosterRepository{staged_roster}.insert(std::move(instance));
    }
}

Result<ResourceCost> SummoningService::calculate_total_banner_cost(const BannerDefinition& banner_definition, const std::int32_t count, const ContentCatalog& content_catalog) {
    ResourceCost   total_resource_cost;
    constexpr auto maximum = std::numeric_limits<std::int64_t>::max();
    //iterate through the banner and calculate the total cost
    for (const auto& [resource_id, amount] : banner_definition.m_cost_per_summon.m_amounts) {
        //check if cost and resource type is valid
        if (amount < 0 || !content_catalog.find(resource_id)) {
            return Error{
                .m_code = Error_code::invalid_argument,
                .m_message = "Summon cost must use known resources and non-negative amounts."
            };
        }
        //check for an integer overflow
        if (amount > (maximum / count)) {
            return Error{
                .m_code = Error_code::invalid_content,
                .m_message = "Total summon cost would overflow."
            };
        }
        total_resource_cost.m_amounts.emplace(resource_id, amount * count);
    }
    return total_resource_cost;
}

bool SummoningService::is_banner_unlocked(const BannerDefinition& banner_definition, const GameState& game_state) {
    const bool is_unlocked = std::ranges::any_of(
        banner_definition.m_required_unlocks,
        [&game_state](const UnlockID& unlock_id) {
            return game_state.m_progression.m_unlocks.contains(unlock_id);
        }
    );
    return is_unlocked;
}