#include "ground/market_engine/pipeline/pipeline.h"

#include <gtest/gtest.h>

namespace
{

PipelineConfig MakeMockConfig()
{
    PipelineConfig config = {};
    config.fetch.source = FetchSource::Mock;
    config.fetch.input.mock.seed = 42;
    config.runId = 1;
    const char *exportPath = "pipeline_test_output.bin";
    for (u32 i = 0; i < kMaxPathLen && exportPath[i] != '\0'; ++i)
    {
        config.exportPath[i] = exportPath[i];
    }
    return config;
}

} // namespace

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
