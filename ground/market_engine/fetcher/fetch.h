/// Fetch stage: loads raw market records from mock or file sources.
#pragma once

#include "ground/market_engine/core/core.h"

/// Dispatches to mock or file fetch based on config.source.
[[nodiscard]] bool Fetch(const FetchConfig& config, RawMarketData& out);
/// Synthesizes a fixed-size deterministic quote universe from a PRNG seed.
[[nodiscard]] bool FetchFromMock(const MockFetchInput& input, RawMarketData& out);
/// Reads a binary raw_market_data fixture from disk into out.
[[nodiscard]] bool FetchFromFile(const FileFetchInput& input, RawMarketData& out);
