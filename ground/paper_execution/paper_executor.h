#pragma once

#include "ground/market_engine/core/core.h"

#include <string>

static constexpr u32 kMaxPortfolioPositions = 256;

struct Portfolio
{
    u32 schemaVersion;
    u64 lastRunId;
    u32 count;
    u32 symbolId[kMaxPortfolioPositions];
    f64 qty[kMaxPortfolioPositions];
    f64 avgCost[kMaxPortfolioPositions];
};

[[nodiscard]] bool RunPaperExecution(const std::string &candidatesPath,
                                     const std::string &portfolioPath);
