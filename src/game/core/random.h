#ifndef PROJECTSKYLAKE_RANDOM_H
#define PROJECTSKYLAKE_RANDOM_H

#include "results.h"
#include "types.h"

#include <string>
#include <cstdint>
#include <memory>

struct StreamKey {
    std::string   m_domain;
    std::uint64_t m_instance{};
    std::uint32_t m_algorithm_version{1};
};

class IRandomStream {
public:
    virtual ~IRandomStream() = default;
    virtual std::uint64_t                  next_u64() = 0;
    virtual std::unique_ptr<IRandomStream> clone() const = 0;
    virtual Result<std::uint64_t>          uniform_u64(std::uint64_t exclusive_upper) = 0;
    virtual Result<bool>                   roll(BasisPoints chance) = 0;
};

class RandomStreamFactory {
public:
    Result<std::unique_ptr<IRandomStream>> make_stream(Seed master, const StreamKey&) const;
};

#endif //PROJECTSKYLAKE_RANDOM_H
