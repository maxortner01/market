/// Pipeline orchestrator — every market_engine run flows through these stages in order.
#include "ground/market_engine/pipeline/pipeline.h"

#include "ground/market_engine/adapt/adapt.h"
#include "ground/market_engine/analysis/analysis.h"
#include "ground/market_engine/export/export.h"
#include "ground/market_engine/fetcher/fetch.h"
#include "ground/market_engine/risk/risk.h"
#include "ground/util/monotonic_timer.h"

#include <algorithm>
#include <cstring>
#include <time.h>

namespace
{

/// Returns the current wall-clock time in nanoseconds for run metadata.
u64 WallClockNs()
{
    timespec ts = {};
    clock_gettime(CLOCK_REALTIME, &ts);
    return static_cast<u64>(ts.tv_sec) * 1000000000ULL + static_cast<u64>(ts.tv_nsec);
}

} // namespace

PipelineStatus RunPipeline(const FetchConfig& fetch, const std::string& exportPath, u64 runId,
                           BuyCandidates& out)
{
    PipelineConfig config = {};
    config.fetch = fetch;
    config.runId = runId;
    config.exportPath = exportPath;
    return RunPipeline(config, out);
}

PipelineStatus RunPipeline(const PipelineConfig& config, BuyCandidates& out)
{
    out = {};
    out.schemaVersion = kSchemaVersion;
    out.runId = config.runId;
    out.generatedAtNs = WallClockNs();

    Timer timer;

    // --- Fetch: pull raw records (mock or file) ---
    RawMarketData raw = {};
    timer.Start();
    if (!Fetch(config.fetch, raw))
    {
        return PipelineStatus::FailFetch;
    }
    out.fetchDurationNs = timer.ElapsedNs();

    // --- InputAdapt: reshape into algorithm-friendly SoA snapshot ---
    MarketSnapshot snapshot = {};
    timer.Start();
    if (!InputAdapt(raw, snapshot))
    {
        return PipelineStatus::FailInputAdapt;
    }
    out.inputAdaptDurationNs = timer.ElapsedNs();

    // --- Algorithm stages ---
    AnalysisState analysis = {};
    timer.Start();
    if (!RunMarketAnalysis(snapshot, analysis))
    {
        return PipelineStatus::FailMarketAnalysis;
    }
    out.marketAnalysisDurationNs = timer.ElapsedNs();

    RiskState risk = {};
    timer.Start();
    if (!RunRiskAnalysis(analysis, risk))
    {
        return PipelineStatus::FailRiskAnalysis;
    }
    out.riskAnalysisDurationNs = timer.ElapsedNs();

    // --- OutputAdapt: map risk state into BuyCandidates layout ---
    timer.Start();
    if (!OutputAdapt(risk, out))
    {
        return PipelineStatus::FailOutputAdapt;
    }
    out.outputAdaptDurationNs = timer.ElapsedNs();

    // --- Export: write buy_candidates.bin + audit sidecar ---
    timer.Start();
    if (!Export(config.exportPath, out))
    {
        return PipelineStatus::FailExport;
    }
    out.exportDurationNs = timer.ElapsedNs();

    return PipelineStatus::Ok;
}
