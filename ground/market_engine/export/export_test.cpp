#include "ground/market_engine/export/export.h"

#include <gtest/gtest.h>

TEST(ExportTest, WriteAndReadBuyCandidatesRoundTrip)
{
    BuyCandidates original = {};
    original.schemaVersion = kSchemaVersion;
    original.runId = 99;
    original.generatedAtNs = 123456789;
    original.count = 2;
    original.symbolId[0] = 1000;
    original.symbolId[1] = 1001;
    original.score[0] = 10.5;
    original.score[1] = 20.5;
    original.maxNotional[0] = 10000.0;
    original.maxNotional[1] = 10000.0;

    const std::string path = "buy_candidates_test.bin";
    ASSERT_TRUE(WriteBuyCandidates(path, original));

    BuyCandidates loaded = {};
    ASSERT_TRUE(ReadBuyCandidates(path, loaded));

    EXPECT_EQ(loaded.schemaVersion, original.schemaVersion);
    EXPECT_EQ(loaded.runId, original.runId);
    EXPECT_EQ(loaded.count, original.count);
    EXPECT_DOUBLE_EQ(loaded.score[1], original.score[1]);
}
