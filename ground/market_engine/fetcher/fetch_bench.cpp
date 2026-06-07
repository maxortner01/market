#include "ground/market_engine/fetcher/fetch.h"

#include <benchmark/benchmark.h>

static void BM_FetchFromMock(benchmark::State &state)
{
    MockFetchInput input = {};
    input.seed = 42;
    for (auto _ : state)
    {
        RawMarketData raw = {};
        const bool ok = FetchFromMock(input, raw);
        benchmark::DoNotOptimize(&ok);
        benchmark::DoNotOptimize(&raw.recordCount);
    }
}

BENCHMARK(BM_FetchFromMock);
