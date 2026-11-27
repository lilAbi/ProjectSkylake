#ifndef PROJECTSKYLAKE_FILESYSTEMSAVESTORE_H
#define PROJECTSKYLAKE_FILESYSTEMSAVESTORE_H

#include "save_store.h"

#include <filesystem>

// The Godot adapter passes a globalized user:// directory to this C++ store.
// Single runtime writer. Slot 0..99; writes replace via a temporary sibling.
class FilesystemSaveStore final : public ISaveStore {
public:
    explicit FilesystemSaveStore(std::filesystem::path directory) : m_directory(std::move(directory)) {}
    Result<std::vector<std::byte>> read(SaveSlot slot) const override;
    Result<void>                   write_replace(SaveSlot slot, std::span<const std::byte> bytes) override;
    Result<void>                   erase(SaveSlot slot) override;
    Result<std::vector<SaveSlot>>  list() const override;

private:
    std::filesystem::path path(SaveSlot slot) const;
    std::filesystem::path m_directory;
};

#endif //PROJECTSKYLAKE_FILESYSTEMSAVESTORE_H
