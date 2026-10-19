#ifndef PROJECTSKYLAKE_SAVE_STORE_H
#define PROJECTSKYLAKE_SAVE_STORE_H

#include "game/core/results.h"
#include "game/core/types.h"

#include <span>
#include <vector>

class ISaveStore {
public:
    virtual ~ISaveStore() = default;
    virtual Result<std::vector<std::byte>> read(SaveSlot) const = 0;
    virtual Result<void>                   write_replace(SaveSlot, std::span<const std::byte>) = 0;
    virtual Result<void>                   erase(SaveSlot) = 0;
    virtual Result<std::vector<SaveSlot>>  list() const = 0;
};

#endif //PROJECTSKYLAKE_SAVE_STORE_H
