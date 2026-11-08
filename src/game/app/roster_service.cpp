#include "roster_service.h"
#include "service_context.h"
#include "game/state/game_state.h"
#include <ranges>

RosterView RosterService::get_roster_view() const {
    RosterView roster_view;
    roster_view.m_characters.reserve(m_context.m_game_state.m_roster.m_characters.size());
    roster_view.m_teams.reserve(m_context.m_game_state.m_teams.m_teams.size());
    //extract a "CharacterView" from each character in the players roster
    for (const auto& character : m_context.m_game_state.m_roster.m_characters | std::views::values) {
        roster_view.m_characters.push_back(
            CharacterSummaryView{
                .m_id = character.m_id,
                .m_display_name = character.m_generated.m_display_name,
                .m_class_id = character.m_generated.m_class_id,
                .m_level = character.m_progression.m_level
            }
        );
    }
    //copy the players current teams
    for (const auto& team : m_context.m_game_state.m_teams.m_teams | std::views::values) {
        roster_view.m_teams.push_back(team);
    }
    return roster_view;
}

Result<CharacterDetailsView> RosterService::get_detailed_character_view(const CharacterID character_id) const {
    const auto& characters = m_context.m_game_state.m_roster.m_characters;
    const auto  itr = characters.find(character_id);
    if (itr == characters.end()) {
        return Error{
            .m_code = Error_code::not_found,
            .m_message = "Character not found"
        };
    }
    //return character metadata if found
    const auto& [m_id, m_generated, m_progression, m_equipment] = itr->second;
    return CharacterDetailsView{
        .m_summary = {
            .m_id = m_id,
            .m_display_name = m_generated.m_display_name,
            .m_class_id = m_generated.m_class_id,
            .m_level = m_progression.m_level
        },
        .m_stats = m_generated.m_rolled_stats,
        .m_traits = m_generated.m_traits,
        .m_tree = m_generated.m_skill_tree,
        .m_progression = m_progression,
        .m_equipment = m_equipment,
    };
}

Result<TeamID> RosterService::create_team(std::string name, const std::int32_t slots) {
    //check if executing via EventBus
    const auto result = check_service_mutation(m_context);
    if (const auto* error = std::get_if<Error>(&result); error) {
        return *error;
    }
    //grab next team id and check if its been used
    const auto next_team_id = m_context.m_game_state.m_identities.m_next_team;
    if (m_context.m_game_state.m_teams.m_teams.contains(TeamID{next_team_id})) {
        return Error{
            .m_code = Error_code::invalid_phase,
            .m_message = "new TeamID already in use."
        };
    }
    //make a copy and add a new team
    auto         team_state_copy = m_context.m_game_state.m_teams;
    const TeamID new_team_id{next_team_id};
    team_state_copy.m_teams.emplace(
        new_team_id,
        Team{
            .m_id = new_team_id,
            .m_name = std::move(name),
            .m_slots = std::vector<std::optional<CharacterID>>(static_cast<std::size_t>(slots))
        }
    );
    //update generation counter
    m_context.m_game_state.m_identities.m_next_team += 1;
    this->commit_teams(m_context, team_state_copy);
    return new_team_id;
}

Result<void> RosterService::rename_team(const TeamID team_id, std::string name) {
    //check if executing via EventBus
    const auto result = check_service_mutation(m_context);
    if (const auto* error = std::get_if<Error>(&result); error) {
        return *error;
    }
    //look for team obj
    const auto& team_container = m_context.m_game_state.m_teams.m_teams;
    const auto  found_team = team_container.find(team_id);
    //check if team exist
    if (found_team == team_container.end()) {
        return Error{
            .m_code = Error_code::not_found,
            .m_message = "Team not found."
        };
    }
    //return if name already matching
    if (found_team->second.m_name == name) {
        return std::monostate{};
    }
    //make a copy of the current team state
    auto team_state_copy = m_context.m_game_state.m_teams;
    //rename the requested team
    team_state_copy.m_teams.at(team_id).m_name = std::move(name);
    //commit change
    this->commit_teams(m_context, team_state_copy);
    return std::monostate{};
}

Result<void> RosterService::set_team_member(TeamID team_id, const std::int32_t slot, std::optional<CharacterID> character_id) {
    //check if executing via EventBus
    const auto result = check_service_mutation(m_context);
    if (const auto* error = std::get_if<Error>(&result); error) {
        return *error;
    }
    //look for requested team
    const auto found_team = m_context.m_game_state.m_teams.m_teams.find(team_id);
    if (found_team == m_context.m_game_state.m_teams.m_teams.end()) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Team not found."
        };
    }
    const auto& team = found_team->second;
    //check if request slot is in range within the team
    if (slot < 0 || static_cast<std::size_t>(slot) >= team.m_slots.size()) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Team slot is out of range."
        };
    }
    //check if character is in the players roster
    if (character_id && !m_context.m_game_state.m_roster.m_characters.contains(*character_id)) {
        return Error{
            .m_code = Error_code::not_found,
            .m_message = "Character not in roster."
        };
    }
    //if member is already set just ignore it
    if (team.m_slots[static_cast<std::size_t>(slot)] == character_id) {
        return std::monostate{};
    }
    //check if the character already occupies a spot within the team
    if (character_id && std::ranges::find(team.m_slots, character_id) != team.m_slots.end()) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "A character can only occupy one slot within the team."
        };
    }
    //check if the team and characters within the team are already in use
    if ( this->is_team_busy(m_context, team) || this->is_team_member_busy(m_context, team) ) {
        return Error{
            .m_code = Error_code::busy,
            .m_message = "This team has an active assignment."
        };
    }
    //make a copy of the current team state
    auto team_state_copy = m_context.m_game_state.m_teams;
    //assign incoming character to the requested slot
    team_state_copy.m_teams.at(team_id).m_slots[static_cast<std::size_t>(slot)] = character_id;
    //commit change
    this->commit_teams(m_context, team_state_copy);
    return std::monostate{};
}

Result<void> RosterService::delete_team(TeamID team_id) {
    //check if executing via EventBus
    const auto result = check_service_mutation(m_context);
    if (const auto* error = std::get_if<Error>(&result); error) {
        return *error;
    }
    const auto found_team = m_context.m_game_state.m_teams.m_teams.find(team_id);
    if ( found_team == m_context.m_game_state.m_teams.m_teams.end() ) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Team not found."
        };
    }
    //check if the team and characters within the team are already in use
    if ( this->is_team_busy(m_context, found_team->second) ) {
        return Error{
            .m_code = Error_code::busy,
            .m_message = "This team has an active assignment."
        };
    }
    //make a copy of the current team state
    auto team_state_copy = m_context.m_game_state.m_teams;
    //delete requested team
    team_state_copy.m_teams.erase(team_id);
    //commit change
    this->commit_teams(m_context, team_state_copy);
    return std::monostate{};
}

void RosterService::commit_teams(const ServiceContext& context, TeamState& new_team_state) {
    context.m_game_state.m_teams.m_teams.swap(new_team_state.m_teams);
    ++context.m_revision.m_value;
    auto is_commited = notify_on_roster_changed(context);
}

bool RosterService::is_team_busy(const ServiceContext& context, const Team& team_state) {
    //check if the team is in use
    const bool is_team_in_use = std::any_of(
        context.m_game_state.m_idle.m_assignments.begin(),
        context.m_game_state.m_idle.m_assignments.end(),
        [&team_state](const IdleAssignment& assignment) {
            return assignment.m_team_id == team_state.m_id;
        }
    );
   return is_team_in_use;
}
bool RosterService::is_team_member_busy(const ServiceContext& context, const Team& team_state) {
    //check if character is already in use within a battle or idle battle
    const bool is_character_in_use = std::any_of(
        team_state.m_slots.begin(),
        team_state.m_slots.end(),
        [&context](const auto& character_id) {
            return character_id &&
                   (context.m_reservations.m_battle_characters.contains(*character_id) ||
                    context.m_reservations.m_idle_characters.contains(*character_id));
        }
    );
    return is_character_in_use;
}