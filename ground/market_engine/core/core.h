/// Core types shared across market_engine pipeline stages.
#pragma once

#include "ground/util/types.h"

#include <string>

static constexpr u32 kMaxPathLen = 256;
static constexpr u32 kSchemaVersion = 1;

enum class FetchSource : u8
{
    Mock = 0,
    File = 1,
};

struct MockFetchInput
{
    u32 seed;
};

struct FileFetchInput
{
    std::string path;
};

// Active member selected by FetchConfig::source (mock vs file.path).
struct FetchInput
{
    MockFetchInput mock;
    FileFetchInput file;
};

struct FetchConfig
{
    FetchSource source;
    FetchInput input;
};

struct PipelineConfig
{
    FetchConfig fetch;
    char exportPath[kMaxPathLen];
    u64 runId;
};

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

static constexpr u32 kMaxRawRecords = 8192;

struct RawMarketData
{
    u32 recordCount;
    u32 symbolId[kMaxRawRecords];
    f64 lastPrice[kMaxRawRecords];
    u64 volume[kMaxRawRecords];
    u64 timestampNs[kMaxRawRecords];
};

static constexpr u32 kMaxSymbols = 8192;

struct MarketSnapshot
{
    u32 count;
    u32 symbolId[kMaxSymbols];
    f64 lastPrice[kMaxSymbols];
    u64 volume[kMaxSymbols];
    u64 timestampNs[kMaxSymbols];
};

static constexpr u32 kMaxAnalysisSymbols = kMaxSymbols;

struct AnalysisState
{
    u32 count;
    u32 symbolId[kMaxAnalysisSymbols];
    f64 score[kMaxAnalysisSymbols];
};

static constexpr u32 kMaxRiskSymbols = kMaxAnalysisSymbols;

struct RiskState
{
    u32 count;
    u32 symbolId[kMaxRiskSymbols];
    f64 score[kMaxRiskSymbols];
    f64 maxNotional[kMaxRiskSymbols];
};

static constexpr u32 kMaxCandidates = 256;

struct BuyCandidates
{
    u32 schemaVersion;
    u64 runId;
    u64 generatedAtNs;
    u32 count;

    u32 symbolId[kMaxCandidates];
    f64 score[kMaxCandidates];
    f64 maxNotional[kMaxCandidates];

    u64 fetchDurationNs;
    u64 inputAdaptDurationNs;
    u64 marketAnalysisDurationNs;
    u64 riskAnalysisDurationNs;
    u64 outputAdaptDurationNs;
    u64 exportDurationNs;
};
