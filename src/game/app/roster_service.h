#ifndef PROJECTSKYLAKE_ROSTER_SERVICE_H
#define PROJECTSKYLAKE_ROSTER_SERVICE_H

#include "views.h"

struct ServiceContext;

class RosterService {
public:
    explicit RosterService(ServiceContext& context) : m_context(context) {};
    ~RosterService() = default;
    [[nodiscard]] RosterView                   get_roster_view() const;
    [[nodiscard]] Result<CharacterDetailsView> get_detailed_character_view(CharacterID character_id) const;
    Result<TeamID>                             create_team(std::string name, std::int32_t slots);
    Result<void>                               rename_team(TeamID team_id, std::string name);
    Result<void>                               set_team_member(TeamID team_id, std::int32_t slot, std::optional<CharacterID> character_id);
    Result<void>                               delete_team(TeamID team_id);

private:
    //todo: might not be needed
    void commit_teams(const ServiceContext& context, TeamState& new_team_state);
    bool is_team_busy(const ServiceContext& context, const Team& team_state);
    bool is_team_member_busy(const ServiceContext& context, const Team& team_state);

private:
    ServiceContext& m_context;
};

#endif //PROJECTSKYLAKE_ROSTER_SERVICE_H
