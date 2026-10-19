#ifndef PROJECTSKYLAKE_SAVE_MIGRATION_H
#define PROJECTSKYLAKE_SAVE_MIGRATION_H
#include "save_types.h"
#include "game/core/results.h"

class SaveMigration {
public:
    virtual ~SaveMigration() = default;
    virtual std::uint32_t from_version() const = 0;
    virtual std::uint32_t to_version() const = 0;
    virtual Result<SaveDocument> migrate(const SaveDocument&) const = 0;
};

#endif //PROJECTSKYLAKE_SAVE_MIGRATION_H
