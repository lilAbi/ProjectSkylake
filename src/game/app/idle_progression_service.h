#ifndef PROJECTSKYLAKE_IDLE_PROGRESSION_SERVICE_H
#define PROJECTSKYLAKE_IDLE_PROGRESSION_SERVICE_H

#include "views.h"

#include <chrono>

struct ServiceContext;

class IDleProgressionService {
public:
    explicit IDleProgressionService(ServiceContext&);
    IdleView                 view() const;
    Result<IdleAssignmentID> assign(TeamID, IdleActivityID);
    Result<void>             unassign(IdleAssignmentID);
    Result<void>             reconcile(std::chrono::sys_seconds now);
    Result<ResourceBundle>   preview(std::chrono::sys_seconds now) const;
    Result<ReceiptID>        claim(std::chrono::sys_seconds now);

private:
    ServiceContext& m_context;
};

#endif //PROJECTSKYLAKE_IDLE_PROGRESSION_SERVICE_H
