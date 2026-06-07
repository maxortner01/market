#include "ground/market_engine/pipeline/pipeline.h"

#include <gtest/gtest.h>

namespace
{

/// Builds a PipelineConfig that uses mock fetch with seed 42.
PipelineConfig MakeMockConfig()
{
    PipelineConfig config = {};
    config.fetch.source = FetchSource::Mock;
    config.fetch.input.mock.seed = 42;
    config.runId = 1;
    config.exportPath = "pipeline_test_output.bin";
    return config;
}

} // namespace

/// Verifies a mock run completes and records non-zero stage timings.
TEST(PipelineTest, RunPipelineProducesCandidatesWithMockFetch)
{
    PipelineConfig config = MakeMockConfig();
    BuyCandidates candidates = {};
    const PipelineStatus status = RunPipeline(config, candidates);

    EXPECT_EQ(status, PipelineStatus::Ok);
    EXPECT_EQ(candidates.schemaVersion, kSchemaVersion);
    EXPECT_EQ(candidates.runId, 1u);
    EXPECT_EQ(candidates.count, 8u);
    EXPECT_GT(candidates.fetchDurationNs, 0u);
}

/// Verifies stable symbol ids, scores, and notional caps for seed 42.
TEST(PipelineTest, GoldenCandidateFieldsForSeed42)
{
    PipelineConfig config = MakeMockConfig();
    BuyCandidates candidates = {};
    ASSERT_EQ(RunPipeline(config, candidates), PipelineStatus::Ok);

    EXPECT_EQ(candidates.count, 8u);
    EXPECT_EQ(candidates.symbolId[0], 1000u);
    EXPECT_EQ(candidates.symbolId[7], 1007u);
    EXPECT_GT(candidates.score[0], 0.0);
    EXPECT_DOUBLE_EQ(candidates.maxNotional[0], 10000.0);
}
