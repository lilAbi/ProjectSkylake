#ifndef PROJECTSKYLAKE_CONTENT_SOURCE_H
#define PROJECTSKYLAKE_CONTENT_SOURCE_H

#include "catalog_data.h"
#include "game/core/results.h"

class IContentSource {
public:
    virtual ~IContentSource() = default;
    [[nodiscard]] virtual Result<CatalogData> load() = 0;
};


#endif //PROJECTSKYLAKE_CONTENT_SOURCE_H
