#include "ground/market_engine/fetcher/fetch.h"

#include "ground/util/log.h"

#include <fstream>

namespace
{

// Cheap integer hash for deterministic mock price generation.
u32 MixSeed(u32 value)
{
    value ^= value << 13;
    value ^= value >> 17;
    value ^= value << 5;
    return value;
}

f64 PriceFromSeed(u32 seed, u32 index)
{
    const u32 mixed = MixSeed(seed + index * 2654435761u);
    const f64 base = static_cast<f64>(mixed % 100000u) / 100.0;
    return base + 1.0;
}

} // namespace

bool FetchFromMock(const MockFetchInput &input, RawMarketData &out)
{
    // Fixed-size mock universe so tests and benchmarks have stable shape.
    out.recordCount = 8;
    u32 seed = input.seed;
    for (u32 i = 0; i < out.recordCount; ++i)
    {
        out.symbolId[i] = 1000 + i;
        out.lastPrice[i] = PriceFromSeed(seed, i);
        out.volume[i] = static_cast<u64>(1000 + i * 10);
        out.timestampNs[i] = static_cast<u64>(1'700'000'000'000'000'000ULL + i);
        seed = MixSeed(seed + i);
    }
    return true;
}

bool FetchFromFile(const FileFetchInput &input, RawMarketData &out)
{
    if (input.path.empty())
    {
        LOG_ERROR("FetchFromFile: path is empty");
        return false;
    }

    std::ifstream file(input.path, std::ios::binary);
    if (!file.is_open())
    {
        LOG_ERROR("FetchFromFile: failed to open %s", input.path.c_str());
        return false;
    }

    // Fixture layout: count followed by SoA columns.
    file.read(reinterpret_cast<char *>(&out.recordCount), sizeof(out.recordCount));
    if (!file)
    {
        LOG_ERROR("FetchFromFile: failed to read record count from %s", input.path.c_str());
        return false;
    }

    if (out.recordCount > kMaxRawRecords)
    {
        LOG_ERROR("FetchFromFile: record count %u exceeds max %u", out.recordCount, kMaxRawRecords);
        return false;
    }

    if (out.recordCount > 0)
    {
        file.read(reinterpret_cast<char *>(out.symbolId), sizeof(u32) * out.recordCount);
        file.read(reinterpret_cast<char *>(out.lastPrice), sizeof(f64) * out.recordCount);
        file.read(reinterpret_cast<char *>(out.volume), sizeof(u64) * out.recordCount);
        file.read(reinterpret_cast<char *>(out.timestampNs), sizeof(u64) * out.recordCount);
        if (!file)
        {
            LOG_ERROR("FetchFromFile: truncated file %s", input.path.c_str());
            return false;
        }
    }

    return true;
}

bool Fetch(const FetchConfig &config, RawMarketData &out)
{
    switch (config.source)
    {
    case FetchSource::Mock:
        return FetchFromMock(config.input.mock, out);
    case FetchSource::File:
        return FetchFromFile(config.input.file, out);
    }

    LOG_ERROR("Fetch: unknown source %u", static_cast<u8>(config.source));
    return false;
}
