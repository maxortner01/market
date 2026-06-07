#include "ground/market_engine/export/export.h"
#include "ground/market_engine/pipeline/pipeline.h"

#include <gtest/gtest.h>

namespace
{

/// Builds a replay PipelineConfig with seed 42 and the given export path.
PipelineConfig MakeReplayConfig(const std::string& exportPath)
{
    PipelineConfig config = {};
    config.fetch.source = FetchSource::Mock;
    config.fetch.input.mock.seed = 42;
    config.runId = 100;
    config.exportPath = exportPath;
    return config;
}

/// Compares candidate signal fields, ignoring per-run timing metadata.
bool CandidatesEqualIgnoringTimings(const BuyCandidates& left, const BuyCandidates& right)
{
    if (left.schemaVersion != right.schemaVersion)
    {
        return false;
    }
    if (left.runId != right.runId)
    {
        return false;
    }
    if (left.count != right.count)
    {
        return false;
    }

    for (u32 i = 0; i < left.count; ++i)
    {
        if (left.symbolId[i] != right.symbolId[i])
        {
            return false;
        }
        if (left.score[i] != right.score[i])
        {
            return false;
        }
        if (left.maxNotional[i] != right.maxNotional[i])
        {
            return false;
        }
    }
    return true;
}

} // namespace

/// Verifies two runs with the same seed produce identical candidate signals.
TEST(ReplayTest, DeterministicPipelineOutputForSameSeed)
{
    const char* firstPath = "replay_first.bin";
    const char* secondPath = "replay_second.bin";

    BuyCandidates first = {};
    BuyCandidates second = {};
    ASSERT_EQ(RunPipeline(MakeReplayConfig(firstPath), first), PipelineStatus::Ok);
    ASSERT_EQ(RunPipeline(MakeReplayConfig(secondPath), second), PipelineStatus::Ok);

    EXPECT_TRUE(CandidatesEqualIgnoringTimings(first, second));
}
