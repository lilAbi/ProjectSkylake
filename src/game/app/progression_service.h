#ifndef PROJECTSKYLAKE_PROGRESSION_SERVICE_H
#define PROJECTSKYLAKE_PROGRESSION_SERVICE_H

#include "views.h"

struct ServiceContext;

class ProgressionService {
public:
    explicit ProgressionService(ServiceContext&);
    ProgressionView view() const;
    Result<void>    can_enter(FloorNumber, const RegionID&) const;

private:
    ServiceContext& m_context;
};

#endif //PROJECTSKYLAKE_PROGRESSION_SERVICE_H
