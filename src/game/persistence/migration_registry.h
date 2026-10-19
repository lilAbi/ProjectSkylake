#ifndef PROJECTSKYLAKE_MIGRATION_REGISTRY_H
#define PROJECTSKYLAKE_MIGRATION_REGISTRY_H
#include "save_migration.h"
#include "save_types.h"
#include "game/core/results.h"

#include <memory>
#include <vector>

class MigrationRegistry {
public:
    Result<void>         add(std::unique_ptr<SaveMigration>);
    Result<SaveDocument> migrate_to_current(SaveDocument) const;

private:
    std::vector<std::unique_ptr<SaveMigration>> m_migrations;
};

#endif //PROJECTSKYLAKE_MIGRATION_REGISTRY_H
