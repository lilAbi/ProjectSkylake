#include "repositories.h"

// RosterRepository ---------------------------------------------------------
RosterRepository::RosterRepository(RosterState& roster_state) : m_state(roster_state) {};

const CharacterInstance* RosterRepository::find(CharacterID character_id) const {
    const auto found = m_state.m_characters.find(character_id);
}

std::vector<CharacterID> RosterRepository::ids() const {

}

Result<void> RosterRepository::insert(CharacterInstance) {

}

Result<void> RosterRepository::erase(CharacterID) {

}

// InventoryRepository ---------------------------------------------------------
InventoryRepository::InventoryRepository(InventoryState&) {

}

const ItemInstance* InventoryRepository::find(ItemID) const {

}

Result<void> InventoryRepository::insert(ItemInstance) {

}

Result<ItemInstance> InventoryRepository::extract(ItemID) {

}

// TeamRepository ---------------------------------------------------------
TeamRepository::TeamRepository(TeamState&) {

}

const Team* TeamRepository::find(TeamID) const {

}

Result<void> TeamRepository::insert(Team) {

}

Result<void> TeamRepository::replace(Team) {

}

Result<void> TeamRepository::erase(TeamID) {

}