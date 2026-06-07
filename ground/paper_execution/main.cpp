/// paper_execution process entry point.
#include "ground/paper_execution/paper_executor.h"
#include "ground/util/flags.h"
#include "ground/util/log.h"

/// Applies exported buy candidates to the simulated portfolio file.
int main(int argc, char** argv)
{
    FLAGS(
        // buy_candidates.bin from market_engine Export.
        FLAG_DEFAULT_STR(candidatesPath, "buy_candidates.bin");
        // Simulated portfolio state output.
        FLAG_DEFAULT_STR(portfolioPath, "portfolio.bin"););

    // Apply signals and update portfolio.bin (no-op if runId already processed).
    if (!RunPaperExecution(candidatesPath, portfolioPath))
    {
        LOG_ERROR("paper_execution failed");
        return 1;
    }

    LOG("paper_execution updated portfolio at %s", portfolioPath.c_str());
    return 0;
}
