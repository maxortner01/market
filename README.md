# market

C++ trading stack under `ground/`. See [docs/CONVENTIONS.md](docs/CONVENTIONS.md) for coding standards.

## Build

Requires Bazel 7.4+ and clang on Linux.

```bash
sh format.sh
bazel test //ground/...
bazel run //ground/market_engine:market_engine -- --seed=42 --exportPath=out.bin
bazel run //ground/paper_execution:paper_execution -- --candidatesPath=out.bin --portfolioPath=portfolio.bin
```

## Layout

- `ground/util` — typedefs, `Timer`, `FLAGS`, `LOG`
- `ground/market_engine` — pipeline process and stages
- `ground/paper_execution` — paper trading process

## Pipeline

Fetch → InputAdapt → MarketAnalysis → RiskAnalysis → OutputAdapt → Export
