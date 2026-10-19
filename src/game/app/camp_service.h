#ifndef PROJECTSKYLAKE_CAMP_SERVICE_H
#define PROJECTSKYLAKE_CAMP_SERVICE_H
#include "views.h"

class ServiceContext;

class CampService {
public:
    explicit CampService(ServiceContext&);
    CampView           view() const;
    Result<BuildingID> construct(BuildingDefinitionID);
    Result<void>       upgrade(BuildingID);

private:
    ServiceContext& m_context;
};

#endif //PROJECTSKYLAKE_CAMP_SERVICE_H
