#ifndef PROJECTSKYLAKE_IDENTITY_COUNTERS_H
#define PROJECTSKYLAKE_IDENTITY_COUNTERS_H
#include <cstdint>

struct IdentityCounters {
    std::int64_t m_next_character{1};
    std::int64_t m_next_item{1};
    std::int64_t m_next_team{1};
    std::int64_t m_next_building{1};
    std::int64_t m_next_battle{1};
    std::int64_t m_next_receipt{1};
    std::int64_t m_next_idle_assignment{1};
};

#endif //PROJECTSKYLAKE_IDENTITY_COUNTERS_H
