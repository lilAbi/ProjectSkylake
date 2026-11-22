#include "skill_tree_generator.h"

#include "game/content/catalog.h"
#include "game/core/random.h"

Result<SkillTree> SkillTreeGenerator::generate(ClassID class_id, std::span<const TraitID> traits, const ContentCatalog& content_catalog, IRandomStream& skill_stream_rng) const {
    // grab class definition
    const auto* class_definition = content_catalog.find(class_id);
    if (!class_definition) {
        return Error{
            .m_code = Error_code::not_found,
            .m_message = "Character class not found."
        };
    }
    // grab starting abilities
    auto abilities = class_definition->m_starting_abilities;
    // re-arrange abilities in random order
    for (std::size_t i = abilities.size(); i > 1; --i) {
        auto draw = skill_stream_rng.uniform_u64(i);
        if (const auto* error = std::get_if<Error>(&draw)) {
            return *error;
        }
        std::swap(abilities[i - 1], abilities[std::get<std::uint64_t>(draw)]);
    }
    SkillTree    tree;
    std::int64_t next = 1;
    for (const auto& ability : abilities) {
        if (!content_catalog.find(ability)) {
            return Error{
                .m_code = Error_code::invalid_content,
                .m_message = "Missing starting ability."
            };
        }
        tree.m_nodes.push_back(
            SkillNode{
                .m_id = SkillNodeID{
                    .m_value = next++
                },
                .m_ability = ability,
                .m_prerequisites = {},
                .m_point_cost = 0
            }
        );
    }
    return tree;
}