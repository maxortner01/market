#include "ground/market_engine/pipeline/pipeline.h"

#include <benchmark/benchmark.h>

namespace
{

/// Builds a PipelineConfig tuned for benchmark iteration overhead.
PipelineConfig MakeBenchConfig()
{
    PipelineConfig config = {};
    config.fetch.source = FetchSource::Mock;
    config.fetch.input.mock.seed = 7;
    config.runId = 99;
    config.exportPath = "pipeline_bench_output.bin";
    return config;
}

} // namespace

/// Measures end-to-end RunPipeline latency with mock fetch.
static void BM_RunPipeline(benchmark::State& state)
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
