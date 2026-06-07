/// Paper execution: applies buy candidates to a simulated portfolio.
#pragma once

#include "ground/market_engine/core/core.h"

#include <string>

/// Max open positions the paper portfolio may hold.
static constexpr u32 kMaxPortfolioPositions = 256;

/// Simulated holdings updated by paper execution after each pipeline run.
struct Portfolio
{
    /// Serialized layout version for portfolio.bin.
    u32 schemaVersion;
    /// Last pipeline runId whose signals were applied (idempotency guard).
    u64 lastRunId;
    /// Number of populated position slots.
    u32 count;
    /// Symbol held per position.
    u32 symbolId[kMaxPortfolioPositions];
    /// Simulated fill quantity per position.
    f64 qty[kMaxPortfolioPositions];
    /// Average cost basis per position (fill price from candidate score).
    f64 avgCost[kMaxPortfolioPositions];
};

/// Reads candidates, updates portfolio.bin, and skips re-application for the same runId.
[[nodiscard]] bool RunPaperExecution(const std::string& candidatesPath,
                                     const std::string& portfolioPath);
