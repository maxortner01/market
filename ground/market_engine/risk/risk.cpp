#include "ground/market_engine/risk/risk.h"

#include "ground/util/log.h"

/// Flat notional cap applied to every symbol until real risk models exist.
static constexpr f64 kDefaultMaxNotional = 10000.0;

bool RunRiskAnalysis(const AnalysisState& in, RiskState& out)
{
    if (in.count > kMaxRiskSymbols)
    {
        LOG_ERROR("RunRiskAnalysis: symbol count %u exceeds max %u", in.count, kMaxRiskSymbols);
        return false;
    }

    // Stub: assign a flat notional cap to every scored symbol.
    out.count = in.count;
    for (u32 i = 0; i < in.count; ++i)
    {
        out.symbolId[i] = in.symbolId[i];
        out.score[i] = in.score[i];
        out.maxNotional[i] = kDefaultMaxNotional;
    }
    return true;
}
