#ifndef PROJECTSKYLAKE_PROGRESSION_SERVICE_H
#define PROJECTSKYLAKE_PROGRESSION_SERVICE_H

#include "views.h"

struct ServiceContext;

class ProgressionService {
public:
    explicit ProgressionService(ServiceContext& context) : m_context(context) {};
    ~ProgressionService() = default;
    ProgressionView view() const;
    Result<void>    can_enter(FloorNumber, const RegionID&) const;

private:
    ServiceContext& m_context;
};

#endif //PROJECTSKYLAKE_PROGRESSION_SERVICE_H
