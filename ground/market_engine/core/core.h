/// Core types shared across market_engine pipeline stages.
#pragma once

#include "ground/util/types.h"

#include <string>

/// Version stamped on serialized BuyCandidates for layout evolution.
static constexpr u32 kSchemaVersion = 1;

/// Selects which FetchInput member is active at runtime.
enum class FetchSource : u8
{
    Mock = 0,
    File = 1,
};

/// Parameters for synthesizing deterministic mock market data.
struct MockFetchInput
{
    /// PRNG seed; same seed yields identical quotes for tests and replay.
    u32 seed;
};

/// Parameters for loading raw market data from a binary fixture file.
struct FileFetchInput
{
    /// Path to raw_market_data.bin fixture on disk.
    std::string path;
};

/// Union-style input bag; active member selected by FetchConfig::source.
struct FetchInput
{
    /// Used when source is Mock.
    MockFetchInput mock;
    /// Used when source is File.
    FileFetchInput file;
};

/// Fetch-stage configuration: source selector plus source-specific input.
struct FetchConfig
{
    /// Mock or file-backed fetch.
    FetchSource source;
    /// Payload for the selected source.
    FetchInput input;
};

/// Full pipeline run configuration passed through every stage.
struct PipelineConfig
{
    /// How and where to pull raw market records.
    FetchConfig fetch;
    /// Output path for buy_candidates.bin (fixed buffer for POD layout).
    std::string exportPath;
    /// Unique id stamped onto this run's exported candidates.
    u64 runId;
};

/// Per-stage failure codes returned by RunPipeline.
enum class PipelineStatus : u8
{
    Ok = 0,
    FailFetch,
    FailInputAdapt,
    FailMarketAnalysis,
    FailRiskAnalysis,
    FailOutputAdapt,
    FailExport,
};

/// Max raw records the fetch stage may produce.
static constexpr u32 kMaxRawRecords = 8192;

/// SoA layout of unnormalized records straight from the fetch stage.
struct RawMarketData
{
    /// Number of populated slots in the parallel arrays below.
    u32 recordCount;
    /// Exchange symbol identifier per record.
    u32 symbolId[kMaxRawRecords];
    /// Last traded price per record.
    f64 lastPrice[kMaxRawRecords];
    /// Traded volume per record.
    u64 volume[kMaxRawRecords];
    /// Event timestamp in nanoseconds per record.
    u64 timestampNs[kMaxRawRecords];
};

/// Max symbols the adapt/analysis stages may carry.
static constexpr u32 kMaxSymbols = 8192;

/// Algorithm-facing market snapshot after input adaptation.
struct MarketSnapshot
{
    /// Number of populated symbol slots.
    u32 count;
    /// Symbol identifier per slot.
    u32 symbolId[kMaxSymbols];
    /// Last price per symbol.
    f64 lastPrice[kMaxSymbols];
    /// Volume per symbol.
    u64 volume[kMaxSymbols];
    /// Timestamp per symbol (ns).
    u64 timestampNs[kMaxSymbols];
};

static constexpr u32 kMaxAnalysisSymbols = kMaxSymbols;

/// Scored symbols produced by the market analysis stage.
struct AnalysisState
{
    /// Number of scored symbols.
    u32 count;
    /// Symbol identifier per slot.
    u32 symbolId[kMaxAnalysisSymbols];
    /// Alpha score per symbol (higher = more attractive).
    f64 score[kMaxAnalysisSymbols];
};

static constexpr u32 kMaxRiskSymbols = kMaxAnalysisSymbols;

/// Risk-sized scores ready for output adaptation.
struct RiskState
{
    /// Number of risk-evaluated symbols.
    u32 count;
    /// Symbol identifier per slot.
    u32 symbolId[kMaxRiskSymbols];
    /// Score carried through from analysis.
    f64 score[kMaxRiskSymbols];
    /// Max notional the strategy may deploy per symbol.
    f64 maxNotional[kMaxRiskSymbols];
};

/// Max buy candidates written to export.
static constexpr u32 kMaxCandidates = 256;

/// Final pipeline output: candidates plus run metadata and stage timings.
struct BuyCandidates
{
    /// Serialized layout version for readers.
    u32 schemaVersion;
    /// Pipeline run that produced these candidates.
    u64 runId;
    /// Wall-clock time (ns) when the run completed.
    u64 generatedAtNs;
    /// Number of candidate slots populated below.
    u32 count;

    /// Symbol to buy per candidate.
    u32 symbolId[kMaxCandidates];
    /// Ranking score per candidate.
    f64 score[kMaxCandidates];
    /// Risk cap (notional) per candidate.
    f64 maxNotional[kMaxCandidates];

    /// Nanoseconds spent in the fetch stage.
    u64 fetchDurationNs;
    /// Nanoseconds spent in input adaptation.
    u64 inputAdaptDurationNs;
    /// Nanoseconds spent in market analysis.
    u64 marketAnalysisDurationNs;
    /// Nanoseconds spent in risk analysis.
    u64 riskAnalysisDurationNs;
    /// Nanoseconds spent in output adaptation.
    u64 outputAdaptDurationNs;
    /// Nanoseconds spent in export.
    u64 exportDurationNs;
};
