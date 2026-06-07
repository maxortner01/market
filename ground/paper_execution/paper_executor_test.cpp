#include "ground/paper_execution/paper_executor.h"

#include "ground/market_engine/export/export.h"

#include <fstream>

#include <gtest/gtest.h>

/// Verifies candidates update portfolio qty and lastRunId on first application.
TEST(PaperExecutorTest, AppliesCandidatesToPortfolio)
{
    BuyCandidates candidates = {};
    candidates.schemaVersion = kSchemaVersion;
    candidates.runId = 42;
    candidates.count = 2;
    candidates.symbolId[0] = 1000;
    candidates.symbolId[1] = 1001;
    candidates.score[0] = 100.0;
    candidates.score[1] = 50.0;
    candidates.maxNotional[0] = 10000.0;
    candidates.maxNotional[1] = 5000.0;

    const std::string candidatesPath = "paper_candidates.bin";
    const std::string portfolioPath = "paper_portfolio.bin";
    ASSERT_TRUE(WriteBuyCandidates(candidatesPath, candidates));
    ASSERT_TRUE(RunPaperExecution(candidatesPath, portfolioPath));

    Portfolio portfolio = {};
    std::ifstream file(portfolioPath, std::ios::binary);
    ASSERT_TRUE(file.is_open());
    file.read(reinterpret_cast<char*>(&portfolio), sizeof(portfolio));

    EXPECT_EQ(portfolio.lastRunId, 42u);
    EXPECT_EQ(portfolio.count, 2u);
    EXPECT_DOUBLE_EQ(portfolio.qty[0], 100.0);
    EXPECT_DOUBLE_EQ(portfolio.qty[1], 100.0);
}

/// Verifies re-running with the same runId leaves the portfolio unchanged.
TEST(PaperExecutorTest, IsIdempotentForSameRunId)
{
    BuyCandidates candidates = {};
    candidates.runId = 7;
    candidates.count = 1;
    candidates.symbolId[0] = 1000;
    candidates.score[0] = 10.0;
    candidates.maxNotional[0] = 1000.0;

    const std::string candidatesPath = "paper_idempotent_candidates.bin";
    const std::string portfolioPath = "paper_idempotent_portfolio.bin";
    ASSERT_TRUE(WriteBuyCandidates(candidatesPath, candidates));
    ASSERT_TRUE(RunPaperExecution(candidatesPath, portfolioPath));

    Portfolio first = {};
    std::ifstream file(portfolioPath, std::ios::binary);
    ASSERT_TRUE(file.is_open());
    file.read(reinterpret_cast<char*>(&first), sizeof(first));
    file.close();

    ASSERT_TRUE(RunPaperExecution(candidatesPath, portfolioPath));

    Portfolio second = {};
    file.open(portfolioPath, std::ios::binary);
    ASSERT_TRUE(file.is_open());
    file.read(reinterpret_cast<char*>(&second), sizeof(second));

    EXPECT_EQ(first.count, second.count);
    EXPECT_DOUBLE_EQ(first.qty[0], second.qty[0]);
}
