#ifndef PROJECTSKYLAKE_SAVE_CODEC_H
#define PROJECTSKYLAKE_SAVE_CODEC_H

#include "dto.h"
#include "save_types.h"
#include "game/core/results.h"
#include "game/core/types.h"

#include <span>

class ISaveCodec {
public:
    virtual ~ISaveCodec() = default;
    virtual Result<std::vector<std::byte>> encode(const SaveGameDTO&) const = 0;
    virtual Result<SaveDocument>           decode_document(std::span<const std::byte>) const = 0;
    virtual Result<SaveGameDTO>            decode_current(const SaveDocument&) const = 0;
    virtual Result<SaveMetadata>           read_metadata(SaveSlot, std::span<const std::byte>) const = 0;
};

#endif //PROJECTSKYLAKE_SAVE_CODEC_H
