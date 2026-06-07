#include "ground/market_engine/analysis/analysis.h"

#include "ground/util/log.h"

bool RunMarketAnalysis(const MarketSnapshot& in, AnalysisState& out)
{
    if (in.count > kMaxAnalysisSymbols)
    {
        LOG_ERROR("RunMarketAnalysis: symbol count %u exceeds max %u", in.count,
                  kMaxAnalysisSymbols);
        return false;
    }

    // Stub: use last price as the score until real alpha exists.
    out.count = in.count;
    for (u32 i = 0; i < in.count; ++i)
    {
        out.symbolId[i] = in.symbolId[i];
        out.score[i] = in.lastPrice[i];
    }
    return true;
}
