#include "ground/util/monotonic_timer.h"

#include <unistd.h>

#include <gtest/gtest.h>

/// Verifies elapsed time grows after a short sleep.
TEST(TimerTest, ElapsedNsIncreasesAfterSleep)
{
    Timer timer;
    timer.Start();
    usleep(10000);
    const u64 elapsedNs = timer.ElapsedNs();
    EXPECT_GE(elapsedNs, 5000000ULL);
}

/// Verifies elapsed time is near zero immediately after Start().
TEST(TimerTest, ElapsedNsIsZeroImmediatelyAfterStart)
{
    Timer timer;
    timer.Start();
    const u64 elapsedNs = timer.ElapsedNs();
    EXPECT_LT(elapsedNs, 10000000ULL);
}
