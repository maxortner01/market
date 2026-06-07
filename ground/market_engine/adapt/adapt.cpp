#include "ground/market_engine/adapt/adapt.h"

#include "ground/util/log.h"

bool InputAdapt(const RawMarketData& in, MarketSnapshot& out)
{
    if (in.recordCount > kMaxSymbols)
    {
        LOG_ERROR("InputAdapt: record count %u exceeds max %u", in.recordCount, kMaxSymbols);
        return false;
    }

    // v0: passthrough copy into the algorithm-facing SoA layout.
    out.count = in.recordCount;
    for (u32 i = 0; i < in.recordCount; ++i)
    {
        out.symbolId[i] = in.symbolId[i];
        out.lastPrice[i] = in.lastPrice[i];
        out.volume[i] = in.volume[i];
        out.timestampNs[i] = in.timestampNs[i];
    }
    return true;
}

bool OutputAdapt(const RiskState& in, BuyCandidates& out)
{
    if (in.count > kMaxCandidates)
    {
        LOG_ERROR("OutputAdapt: candidate count %u exceeds max %u", in.count, kMaxCandidates);
        return false;
    }

    // Copy scored, risk-sized symbols into the export schema (metadata already on out).
    out.count = in.count;
    for (u32 i = 0; i < in.count; ++i)
    {
        out.symbolId[i] = in.symbolId[i];
        out.score[i] = in.score[i];
        out.maxNotional[i] = in.maxNotional[i];
    }
    return true;
}
