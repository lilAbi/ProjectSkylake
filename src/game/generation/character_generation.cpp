#include "character_generation.h"

#include "skill_tree_generator.h"
#include "game/content/catalog.h"
#include "game/core/random.h"

Result<GeneratedCharacter> CharacterGenerator::generate(const CharacterGenerationRequest& request, IRandomStream& character_rng_stream, IRandomStream& skill_rng_stream) const {
    // validate incoming request
    if (request.m_allowed_classes.empty() ||
        request.m_tier != 1 ||
        request.m_metadata.m_generator_version != 1 ||
        request.m_metadata.m_content_version != m_content_catalog.content_version()) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Generator v1 requires a class pool, tier 1 and matching content metadata."
        };
    }
    // roll the summon's class to summon from the banner and validate
    auto class_draw = character_rng_stream.uniform_u64(request.m_allowed_classes.size());
    if (const auto* error = std::get_if<Error>(&class_draw)) {
        return *error;
    }
    // grab class id
    const auto class_id = request.m_allowed_classes[std::get<std::uint64_t>(class_draw)];
    const auto* class_definition = m_content_catalog.find(class_id);
    if (!class_definition) {
        return Error{
            .m_code = Error_code::invalid_content,
            .m_message = "Generator class does not exist."
        };
    }
    // generate result
    GeneratedCharacter result{
        .m_class_id = class_id,
        .m_display_name = class_definition->m_name,
        .m_rolled_stats = class_definition->m_base_stats,
        .m_traits = {},
        .m_skill_tree = {},
        .m_generation = request.m_metadata
    };
    // rolls a value for each stat, each base stat varies by up to 10%
    for (auto& stat : result.m_rolled_stats.m_values) {
        if (stat < 0) {
            return Error{
                .m_code = Error_code::invalid_content,
                .m_message = "Base stats must be nonnegative."
            };
        }
        const auto stat_spread = stat / 10;
        if (stat > std::numeric_limits<std::int64_t>::max() - stat_spread) {
            return Error{
                .m_code = Error_code::overflow,
                .m_message = "Stat roll would overflow."
            };
        }
        // starter balance rule: each base stat varies by up to 10%
        auto draw = character_rng_stream.uniform_u64(static_cast<std::uint64_t>(stat_spread) * 2 + 1);
        if (const auto* error = std::get_if<Error>(&draw)) {
            return *error;
        }
        // set stat
        stat = stat - stat_spread + static_cast<std::int64_t>(std::get<std::uint64_t>(draw));
    }
    // grab a random trait
    if (!class_definition->m_allowed_traits.empty()) {
        // roll value
        auto draw = character_rng_stream.uniform_u64(class_definition->m_allowed_traits.size());
        if (const auto* error = std::get_if<Error>(&draw)) {
            return *error;
        }
        // grab trait
        const auto character_trait = class_definition->m_allowed_traits[std::get<std::uint64_t>(draw)];
        if (!m_content_catalog.find(character_trait)) {
            return Error{
                .m_code = Error_code::invalid_content,
                .m_message = "Generator trait does not exist."
            };
        }
        result.m_traits.push_back(character_trait);
    }
    //generate a skill tree
    auto tree = SkillTreeGenerator().generate(class_id, result.m_traits, m_content_catalog, skill_rng_stream);
    if (const auto* error = std::get_if<Error>(&tree)) {
        return *error;
    }
    result.m_skill_tree = std::get<SkillTree>(std::move(tree));
    return result;
}