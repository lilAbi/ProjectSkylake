#include "random.h"

//SplitMix64, with stable FNV-1a stream-key derivation.
std::uint64_t mix(std::uint64_t value) noexcept {
    value = (value ^ (value >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    value = (value ^ (value >> 27)) * UINT64_C(0x94d049bb133111eb);
    return value ^ (value >> 31);
}

std::uint64_t SplitMixStream::next_u64() {
    m_state += UINT64_C(0x9e3779b97f4a7c15);
    return mix(m_state);
}

std::unique_ptr<IRandomStream> SplitMixStream::clone() {
    return std::make_unique<SplitMixStream>(*this);
}

Result<std::uint64_t> SplitMixStream::uniform_u64(const std::uint64_t exclusive_upper) {
    if (exclusive_upper == 0) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Random upper bound must be positive."
        };
    }
    // Rejection avoids modulo bias, including for bounds near UINT64_MAX.
    const auto threshold = (std::uint64_t{0} - exclusive_upper) % exclusive_upper;
    for (;;) {
        const auto value = next_u64();
        if (value >= threshold) {
            return value % exclusive_upper;
        }
    }
}

Result<bool> SplitMixStream::roll(const BasisPoints chance) {
    if (chance.m_value < 0 || chance.m_value > 10'000) {
        return Error{
            .m_code = Error_code::invalid_argument,
            .m_message = "Chance must be between 0 and 10,000 basis points."
        };
    }
    const auto value = uniform_u64(10'000);
    return std::get<std::uint64_t>(value) < static_cast<std::uint64_t>(chance.m_value);
}

Result<std::unique_ptr<IRandomStream>> RandomStreamFactory::make_stream(const Seed master, const StreamKey& key) const {
    if (key.m_domain.empty() || key.m_algorithm_version != 1) {
        return Error{
            .m_code = Error_code::unsupported_version,
            .m_message = "Non-empty domain required or algorithm not supported."
        };
    }
    std::uint64_t hash = UINT64_C(14695981039346656037);
    for (const unsigned char byte : key.m_domain) {
        hash = (hash ^ byte) * UINT64_C(1099511628211);
    }
    // Explicit little-endian encoding is part of the versioned algorithm.
    for (unsigned shift = 0; shift < 64; shift += 8) {
        hash = (hash ^ ((key.m_instance >> shift) & 0xff)) * UINT64_C(1099511628211);
    }
    for (unsigned shift = 0; shift < 32; shift += 8) {
        hash = (hash ^ ((key.m_algorithm_version >> shift) & 0xff)) * UINT64_C(1099511628211);
    }
    return std::unique_ptr<IRandomStream>{std::make_unique<SplitMixStream>(mix(master.m_value ^ hash))};
}