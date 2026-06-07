/// Risk analysis stage: applies notional limits to scored symbols.
#pragma once

#include "ground/market_engine/core/core.h"

/// Attaches max-notional caps to each scored symbol for downstream sizing.
[[nodiscard]] bool RunRiskAnalysis(const AnalysisState& in, RiskState& out);
