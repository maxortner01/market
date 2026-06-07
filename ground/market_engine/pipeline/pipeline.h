/// Pipeline orchestrator: runs Fetch through Export and records per-stage timings.
#pragma once

#include "ground/market_engine/core/core.h"

#include <string>

/// Executes all pipeline stages in order and writes export output via config.exportPath.
PipelineStatus RunPipeline(const PipelineConfig& config, BuyCandidates& out);

/// Executes all pipeline stages using std::string paths (preferred at I/O boundaries).
PipelineStatus RunPipeline(const FetchConfig& fetch, const std::string& exportPath, u64 runId,
                           BuyCandidates& out);
