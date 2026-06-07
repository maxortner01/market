/// Market analysis stage: scores symbols from a market snapshot.
#pragma once

#include "ground/market_engine/core/core.h"

/// Produces per-symbol alpha scores from the current market snapshot.
[[nodiscard]] bool RunMarketAnalysis(const MarketSnapshot& in, AnalysisState& out);
