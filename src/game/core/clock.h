#ifndef PROJECTSKYLAKE_CLOCK_H
#define PROJECTSKYLAKE_CLOCK_H

#include <chrono>

class IClock {
public:
    virtual ~IClock() = default;
    virtual std::chrono::sys_seconds wall_now() const = 0;
    virtual std::chrono::steady_clock::time_point steady_now() const = 0;
};

#endif //PROJECTSKYLAKE_CLOCK_H
