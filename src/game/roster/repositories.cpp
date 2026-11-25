#include "repositories.h"

#include <ranges>

// RosterRepository ---------------------------------------------------------
const CharacterInstance* RosterRepository::find(const CharacterID character_id) const {
    const auto found = m_state.m_characters.find(character_id);
    return found == m_state.m_characters.end() ? nullptr : &found->second;
}

std::vector<CharacterID> RosterRepository::ids() const {
    std::vector<CharacterID> result;
    result.reserve(m_state.m_characters.size());
    for (const auto& id : m_state.m_characters | std::views::keys) {
        result.push_back(id);
    }
    return result;
}

Result<void> RosterRepository::insert(CharacterInstance character_instance) {
    if (character_instance.m_id.m_value <= 0) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Character ID must be positive."
        };
    }
    if (!m_state.m_characters.emplace(character_instance.m_id, std::move(character_instance)).second) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Character ID already exists."
        };
    }
    return std::monostate{};
}

Result<void> RosterRepository::erase(const CharacterID character_id) {
    if (m_state.m_characters.erase(character_id) == 0) {
        return Error{
            .m_code = Error_code::not_found,
            .m_message = "Character not found."
        };
    }
    return std::monostate{};
}

// InventoryRepository ---------------------------------------------------------
const ItemInstance* InventoryRepository::find(const ItemID item_id) const {
    const auto found = m_state.m_items.find(item_id);
    return found == m_state.m_items.end() ? nullptr : &found->second;
}

Result<void> InventoryRepository::insert(ItemInstance item_instance) const {
    if (item_instance.m_id.m_value <= 0 || m_state.m_capacity < 0) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Invalid item ID or inventory capacity."
        };
    }
    if (m_state.m_items.contains(item_instance.m_id)) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Item ID already exists."
        };
    }
    if (m_state.m_items.size() >= static_cast<std::uint64_t>(m_state.m_capacity)) {
        return Error{
            .m_code = Error_code::capacity_exceeded,
            .m_message = "Inventory is full."
        };
    }
    const auto id = item_instance.m_id;
    m_state.m_items.emplace(id, std::move(item_instance));
    return std::monostate{};
}

Result<ItemInstance> InventoryRepository::extract(const ItemID item_id) const {
    const auto found = m_state.m_items.find(item_id);
    if (found == m_state.m_items.end()) {
        return Error{
            .m_code = Error_code::not_found,
            .m_message = "Item not found."
        };
    }
    const auto node = m_state.m_items.extract(found);
    return std::move(node.mapped());
}

// TeamRepository ---------------------------------------------------------
const Team* TeamRepository::find(const TeamID team_id) const {
    const auto found = m_state.m_teams.find(team_id);
    return found == m_state.m_teams.end() ? nullptr : &found->second;
}

Result<void> TeamRepository::insert(Team team) {
    if (team.m_id.m_value <= 0) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Team ID must be positive."
        };
    }
    if (!m_state.m_teams.emplace(team.m_id, std::move(team)).second) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Team ID already exists."
        };
    }
    return std::monostate{};
}

Result<void> TeamRepository::replace(Team team) {
    const auto found = m_state.m_teams.find(team.m_id);
    if (found == m_state.m_teams.end()) {
        return Error{
            .m_code = Error_code::not_found,
            .m_message = "Team not found."
        };
    }
    std::swap(found->second, team);
    return std::monostate{};
}

Result<void> TeamRepository::erase(TeamID team_id) {
    if (m_state.m_teams.erase(team_id) == 0) {
        return Error{
            .m_code = Error_code::not_found,
            .m_message = "Team not found."
        };
    }
    return std::monostate{};
}