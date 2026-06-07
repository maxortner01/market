/// market_engine process entry point.
#include "ground/market_engine/pipeline/pipeline.h"
#include "ground/util/flags.h"
#include "ground/util/log.h"

/// Runs the full market_engine pipeline and writes buy_candidates.bin.
int main(int argc, char** argv)
{
    FLAGS(
        // PRNG seed for mock fetch when fetchPath is empty.
        FLAG_DEFAULT(u32, seed, 42);
        // Unique id stamped onto this run's output.
        FLAG_DEFAULT(u64, runId, 1);
        // Where to write buy_candidates.bin.
        FLAG_DEFAULT_STR(exportPath, "buy_candidates.bin");
        // Optional raw_market_data.bin fixture; empty uses mock fetch.
        FLAG_STR(fetchPath););

    FetchConfig fetch = {};
    if (!fetchPath.empty())
    {
        // User supplied a fixture file — read market data from disk.
        fetch.source = FetchSource::File;
        fetch.input.file.path = fetchPath;
    }
    else
    {
        // No fixture — synthesize deterministic mock quotes from seed.
        fetch.source = FetchSource::Mock;
        fetch.input.mock.seed = seed;
    }

    BuyCandidates candidates = {};
    const PipelineStatus status = RunPipeline(fetch, exportPath, runId, candidates);
    if (status != PipelineStatus::Ok)
    {
        LOG_ERROR("market_engine failed with status %u", static_cast<u8>(status));
        return 1;
    }

    LOG("market_engine exported %u candidates to %s", candidates.count, exportPath.c_str());
    return 0;
}
