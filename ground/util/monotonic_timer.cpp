#include "ground/util/monotonic_timer.h"

#include <time.h>

namespace
{

/// Converts a timespec from clock_gettime into nanoseconds.
u64 TimespecToNs(const timespec& ts)
{
    return static_cast<u64>(ts.tv_sec) * 1000000000ULL + static_cast<u64>(ts.tv_nsec);
}

} // namespace

void Timer::Start()
{
    timespec ts = {};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    m_startNs = TimespecToNs(ts);
}

u64 Timer::ElapsedNs() const
{
    timespec ts = {};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    const u64 nowNs = TimespecToNs(ts);
    if (nowNs < m_startNs)
    {
        return 0;
    }
    return nowNs - m_startNs;
}
