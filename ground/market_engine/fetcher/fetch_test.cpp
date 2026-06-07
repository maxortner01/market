#include "ground/market_engine/fetcher/fetch.h"

#include <fstream>

#include <gtest/gtest.h>

namespace
{

/// Writes a minimal binary fixture matching FetchFromFile's expected layout.
void WriteFixtureFile(const char* path, const RawMarketData& data)
{
    std::ofstream file(path, std::ios::binary);
    ASSERT_TRUE(file.is_open());
    file.write(reinterpret_cast<const char*>(&data.recordCount), sizeof(data.recordCount));
    file.write(reinterpret_cast<const char*>(data.symbolId), sizeof(u32) * data.recordCount);
    file.write(reinterpret_cast<const char*>(data.lastPrice), sizeof(f64) * data.recordCount);
    file.write(reinterpret_cast<const char*>(data.volume), sizeof(u64) * data.recordCount);
    file.write(reinterpret_cast<const char*>(data.timestampNs), sizeof(u64) * data.recordCount);
}

/// Builds a four-record RawMarketData for file-fetch tests.
RawMarketData MakeFixtureData()
{
    RawMarketData data = {};
    data.recordCount = 4;
    data.symbolId[0] = 1000;
    data.symbolId[1] = 1001;
    data.symbolId[2] = 1002;
    data.symbolId[3] = 1003;
    data.lastPrice[0] = 150.25;
    data.lastPrice[1] = 200.50;
    data.lastPrice[2] = 75.10;
    data.lastPrice[3] = 300.00;
    data.volume[0] = 1000;
    data.volume[1] = 2000;
    data.volume[2] = 3000;
    data.volume[3] = 4000;
    for (u32 i = 0; i < data.recordCount; ++i)
    {
        data.timestampNs[i] = 1700000000000000000ULL + i;
    }
    return data;
}

} // namespace

/// Verifies the same seed yields identical mock records across calls.
TEST(FetchFromMockTest, ProducesDeterministicRecordsForSeed)
{
    MockFetchInput input = {};
    input.seed = 42;

    RawMarketData first = {};
    RawMarketData second = {};
    ASSERT_TRUE(FetchFromMock(input, first));
    ASSERT_TRUE(FetchFromMock(input, second));

    EXPECT_EQ(first.recordCount, second.recordCount);
    EXPECT_EQ(first.recordCount, 8u);
    EXPECT_EQ(first.symbolId[0], second.symbolId[0]);
    EXPECT_DOUBLE_EQ(first.lastPrice[3], second.lastPrice[3]);
}

/// Verifies a written fixture round-trips through FetchFromFile.
TEST(FetchFromFileTest, ReadsFixtureFile)
{
    const char* path = "fetch_fixture_temp.bin";
    const RawMarketData fixture = MakeFixtureData();
    WriteFixtureFile(path, fixture);

    FileFetchInput input = {};
    input.path = path;

    RawMarketData data = {};
    ASSERT_TRUE(FetchFromFile(input, data));
    EXPECT_EQ(data.recordCount, 4u);
    EXPECT_EQ(data.symbolId[0], 1000u);
}
