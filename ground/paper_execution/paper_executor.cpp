#include "ground/paper_execution/paper_executor.h"

#include "ground/market_engine/export/export.h"
#include "ground/util/log.h"

#include <fstream>

static constexpr u32 kPortfolioSchemaVersion = 1;

bool RunPaperExecution(const std::string &candidatesPath, const std::string &portfolioPath)
{
    BuyCandidates candidates = {};
    if (!ReadBuyCandidates(candidatesPath, candidates))
    {
        return false;
    }

    Portfolio portfolio = {};
    {
        std::ifstream file(portfolioPath, std::ios::binary);
        if (file.is_open())
        {
            file.read(reinterpret_cast<char *>(&portfolio), sizeof(portfolio));
            if (!file)
            {
                LOG_ERROR("RunPaperExecution: failed to read portfolio %s", portfolioPath.c_str());
                return false;
            }
        }
        else
        {
            // First run — start from an empty portfolio.
            portfolio = {};
            portfolio.schemaVersion = kPortfolioSchemaVersion;
        }
    }

    // Already applied this run's signals — nothing to do.
    if (portfolio.lastRunId == candidates.runId)
    {
        return true;
    }

    portfolio.schemaVersion = kPortfolioSchemaVersion;
    portfolio.lastRunId = candidates.runId;
    portfolio.count = candidates.count;

    for (u32 i = 0; i < candidates.count && i < kMaxPortfolioPositions; ++i)
    {
        portfolio.symbolId[i] = candidates.symbolId[i];
        portfolio.avgCost[i] = candidates.score[i];
        // Spend up to maxNotional at the simulated fill price (score).
        portfolio.qty[i] =
            candidates.score[i] > 0.0 ? candidates.maxNotional[i] / candidates.score[i] : 0.0;
    }

    std::ofstream out(portfolioPath, std::ios::binary);
    if (!out.is_open())
    {
        LOG_ERROR("RunPaperExecution: failed to open portfolio %s", portfolioPath.c_str());
        return false;
    }
    out.write(reinterpret_cast<const char *>(&portfolio), sizeof(portfolio));
    if (!out)
    {
        LOG_ERROR("RunPaperExecution: failed to write portfolio %s", portfolioPath.c_str());
        return false;
    }
    return true;
}
