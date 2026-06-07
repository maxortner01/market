/// Adapt stage: reshapes pipeline data between fetch, algorithm, and export layouts.
#pragma once

#include "ground/market_engine/core/core.h"

/// Copies raw fetch records into the algorithm-facing MarketSnapshot SoA.
[[nodiscard]] bool InputAdapt(const RawMarketData& in, MarketSnapshot& out);
/// Maps risk-sized scores into the BuyCandidates export schema.
[[nodiscard]] bool OutputAdapt(const RiskState& in, BuyCandidates& out);
