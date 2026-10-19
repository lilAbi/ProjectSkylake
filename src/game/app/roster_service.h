#ifndef PROJECTSKYLAKE_ROSTER_SERVICE_H
#define PROJECTSKYLAKE_ROSTER_SERVICE_H

#include "views.h"

struct ServiceContext;

class RosterService {
public:
    explicit RosterService(ServiceContext&);
    RosterView                   view() const;
    Result<CharacterDetailsView> character(CharacterID) const;
    Result<TeamID>               create_team(std::string name, std::int32_t slots);
    Result<void>                 rename_team(TeamID, std::string);
    Result<void>                 set_team_member(TeamID, std::int32_t slot, std::optional<CharacterID>);
    Result<void>                 delete_team(TeamID);

private:
    ServiceContext& m_context;
};

#endif //PROJECTSKYLAKE_ROSTER_SERVICE_H
