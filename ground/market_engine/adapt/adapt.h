#pragma once

#include "ground/market_engine/core/core.h"

[[nodiscard]] bool InputAdapt(const RawMarketData &in, MarketSnapshot &out);
[[nodiscard]] bool OutputAdapt(const RiskState &in, BuyCandidates &out);
