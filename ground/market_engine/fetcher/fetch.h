#pragma once

#include "ground/market_engine/core/core.h"

[[nodiscard]] bool Fetch(const FetchConfig &config, RawMarketData &out);
[[nodiscard]] bool FetchFromMock(const MockFetchInput &input, RawMarketData &out);
[[nodiscard]] bool FetchFromFile(const FileFetchInput &input, RawMarketData &out);
