#ifndef PROJECTSKYLAKE_VALIDATION_H
#define PROJECTSKYLAKE_VALIDATION_H

#include "game/core/results.h"

class ContentCatalog;

//all registries must be built before validating references between definitions
[[nodiscard]] Result<void> validate_catalog(const ContentCatalog& catalog);

#endif //PROJECTSKYLAKE_VALIDATION_H
