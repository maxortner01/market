#include "ground/util/monotonic_timer.h"

#include <unistd.h>

#include <gtest/gtest.h>

TEST(TimerTest, ElapsedNsIncreasesAfterSleep)
{
    Timer timer;
    timer.Start();
    usleep(10000);
    const u64 elapsedNs = timer.ElapsedNs();
    EXPECT_GE(elapsedNs, 5000000ULL);
}

TEST(TimerTest, ElapsedNsIsZeroImmediatelyAfterStart)
{
    Timer timer;
    timer.Start();
    const u64 elapsedNs = timer.ElapsedNs();
    EXPECT_LT(elapsedNs, 10000000ULL);
}
