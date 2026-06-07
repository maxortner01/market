/// Replay tool: re-runs the pipeline and optionally diffs output against a golden file.
#include "ground/market_engine/export/export.h"
#include "ground/market_engine/pipeline/pipeline.h"
#include "ground/util/log.h"

namespace
{

bool CandidatesEqualIgnoringTimings(const BuyCandidates &left, const BuyCandidates &right)
{
    if (left.schemaVersion != right.schemaVersion || left.runId != right.runId ||
        left.count != right.count)
    {
        return false;
    }

    for (u32 i = 0; i < left.count; ++i)
    {
        if (left.symbolId[i] != right.symbolId[i] || left.score[i] != right.score[i] ||
            left.maxNotional[i] != right.maxNotional[i])
        {
            return false;
        }
    }
    return true;
}

} // namespace

int main(int argc, char **argv)
{
    const char *expectedPath = (argc >= 2 && argv[1] != nullptr) ? argv[1] : nullptr;

    FetchConfig fetch = {};
    fetch.source = FetchSource::Mock;
    fetch.input.mock.seed = 42;

    BuyCandidates candidates = {};
    const PipelineStatus status = RunPipeline(fetch, "replay_output.bin", 100, candidates);
    if (status != PipelineStatus::Ok)
    {
        LOG_ERROR("replay pipeline failed with status %u", static_cast<u8>(status));
        return 1;
    }

    if (expectedPath != nullptr)
    {
        BuyCandidates expected = {};
        if (!ReadBuyCandidates(expectedPath, expected))
        {
            return 1;
        }
        // Timings differ run-to-run; compare signal fields only.
        if (!CandidatesEqualIgnoringTimings(candidates, expected))
        {
            LOG_ERROR("replay output mismatch for %s", expectedPath);
            return 1;
        }
        LOG("replay matched expected output %s", expectedPath);
        return 0;
    }

    LOG("replay produced %u candidates", candidates.count);
    return 0;
}
