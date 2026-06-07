#include "ground/market_engine/pipeline/pipeline.h"

#include <benchmark/benchmark.h>

namespace
{

PipelineConfig MakeBenchConfig()
{
    PipelineConfig config = {};
    config.fetch.source = FetchSource::Mock;
    config.fetch.input.mock.seed = 7;
    config.runId = 99;
    const char *exportPath = "pipeline_bench_output.bin";
    for (u32 i = 0; i < kMaxPathLen && exportPath[i] != '\0'; ++i)
    {
        config.exportPath[i] = exportPath[i];
    }
    return config;
}

} // namespace

static void BM_RunPipeline(benchmark::State &state)
{
    PipelineConfig config = MakeBenchConfig();
    for (auto _ : state)
    {
        BuyCandidates candidates = {};
        const PipelineStatus status = RunPipeline(config, candidates);
        benchmark::DoNotOptimize(&status);
        benchmark::DoNotOptimize(&candidates.count);
    }
}

BENCHMARK(BM_RunPipeline);
