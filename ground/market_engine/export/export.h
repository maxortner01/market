/// Export pipeline stage: serializes BuyCandidates and writes audit metadata.
#pragma once

#include "ground/market_engine/core/core.h"

#include <string>

struct RunAudit
{
    u32 schemaVersion;
    u64 runId;
    u64 generatedAtNs;
    u32 candidateCount;
    u64 fetchDurationNs;
    u64 inputAdaptDurationNs;
    u64 marketAnalysisDurationNs;
    u64 riskAnalysisDurationNs;
    u64 outputAdaptDurationNs;
    u64 exportDurationNs;
};

[[nodiscard]] bool WriteBuyCandidates(const std::string &path, const BuyCandidates &candidates);
[[nodiscard]] bool ReadBuyCandidates(const std::string &path, BuyCandidates &out);
[[nodiscard]] bool WriteRunAudit(const std::string &path, const RunAudit &audit);
[[nodiscard]] bool Export(const std::string &exportPath, const BuyCandidates &candidates);
[[nodiscard]] bool Export(const char *exportPath, const BuyCandidates &candidates);
