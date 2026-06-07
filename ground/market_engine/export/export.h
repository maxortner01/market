/// Export pipeline stage: serializes BuyCandidates and writes audit metadata.
#pragma once

#include "ground/market_engine/core/core.h"

#include <string>

/// Sidecar record of run metadata and per-stage timings for replay/debug.
struct RunAudit
{
    /// Layout version matching the candidates file.
    u32 schemaVersion;
    /// Pipeline run id.
    u64 runId;
    /// Wall-clock completion time (ns).
    u64 generatedAtNs;
    /// Number of candidates exported.
    u32 candidateCount;
    /// Fetch stage duration (ns).
    u64 fetchDurationNs;
    /// Input adapt stage duration (ns).
    u64 inputAdaptDurationNs;
    /// Market analysis stage duration (ns).
    u64 marketAnalysisDurationNs;
    /// Risk analysis stage duration (ns).
    u64 riskAnalysisDurationNs;
    /// Output adapt stage duration (ns).
    u64 outputAdaptDurationNs;
    /// Export stage duration (ns).
    u64 exportDurationNs;
};

/// Writes a BuyCandidates struct verbatim to a binary file.
[[nodiscard]] bool WriteBuyCandidates(const std::string& path, const BuyCandidates& candidates);
/// Reads a BuyCandidates struct from a binary file written by WriteBuyCandidates.
[[nodiscard]] bool ReadBuyCandidates(const std::string& path, BuyCandidates& out);
/// Writes a RunAudit sidecar for inspecting timings without re-parsing candidates.
[[nodiscard]] bool WriteRunAudit(const std::string& path, const RunAudit& audit);
/// Writes candidates plus an .audit.bin sidecar next to exportPath.
[[nodiscard]] bool Export(const std::string& exportPath, const BuyCandidates& candidates);
/// C-string overload for PipelineConfig's fixed exportPath buffer.
[[nodiscard]] bool Export(const char* exportPath, const BuyCandidates& candidates);
