#include "ground/market_engine/export/export.h"

#include "ground/util/log.h"

#include <fstream>

namespace
{

/// Builds a RunAudit sidecar from the timing and metadata fields on candidates.
RunAudit BuildAudit(const BuyCandidates& candidates)
{
    RunAudit audit = {};
    audit.schemaVersion = candidates.schemaVersion;
    audit.runId = candidates.runId;
    audit.generatedAtNs = candidates.generatedAtNs;
    audit.candidateCount = candidates.count;
    audit.fetchDurationNs = candidates.fetchDurationNs;
    audit.inputAdaptDurationNs = candidates.inputAdaptDurationNs;
    audit.marketAnalysisDurationNs = candidates.marketAnalysisDurationNs;
    audit.riskAnalysisDurationNs = candidates.riskAnalysisDurationNs;
    audit.outputAdaptDurationNs = candidates.outputAdaptDurationNs;
    audit.exportDurationNs = candidates.exportDurationNs;
    return audit;
}

} // namespace

bool WriteBuyCandidates(const std::string& path, const BuyCandidates& candidates)
{
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        LOG_ERROR("WriteBuyCandidates: failed to open %s", path.c_str());
        return false;
    }
    // Whole struct written verbatim — schema_version guards layout evolution.
    file.write(reinterpret_cast<const char*>(&candidates), sizeof(candidates));
    if (!file)
    {
        LOG_ERROR("WriteBuyCandidates: failed to write %s", path.c_str());
        return false;
    }
    return true;
}

bool ReadBuyCandidates(const std::string& path, BuyCandidates& out)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        LOG_ERROR("ReadBuyCandidates: failed to open %s", path.c_str());
        return false;
    }
    file.read(reinterpret_cast<char*>(&out), sizeof(out));
    if (!file)
    {
        LOG_ERROR("ReadBuyCandidates: failed to read %s", path.c_str());
        return false;
    }
    return true;
}

bool WriteRunAudit(const std::string& path, const RunAudit& audit)
{
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        LOG_ERROR("WriteRunAudit: failed to open %s", path.c_str());
        return false;
    }
    file.write(reinterpret_cast<const char*>(&audit), sizeof(audit));
    if (!file)
    {
        LOG_ERROR("WriteRunAudit: failed to write %s", path.c_str());
        return false;
    }
    return true;
}

bool Export(const std::string& exportPath, const BuyCandidates& candidates)
{
    if (!WriteBuyCandidates(exportPath, candidates))
    {
        return false;
    }
    // Sidecar audit file captures timings for replay/debug without re-parsing candidates.
    return WriteRunAudit(exportPath + ".audit.bin", BuildAudit(candidates));
}

bool Export(const char* exportPath, const BuyCandidates& candidates)
{
    if (exportPath == nullptr)
    {
        LOG_ERROR("Export: path is null");
        return false;
    }
    return Export(std::string(exportPath), candidates);
}
