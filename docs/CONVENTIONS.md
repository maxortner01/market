# Software Conventions

Single source of truth for coding standards in this repository.

## Pragmatism

- Prefer the simplest structure that works. Don't extract helpers used once; inline them.
- Use `std::string` (and streams) at I/O boundaries instead of hand-rolled char copying.
- Hot pipeline paths stay POD/SoA; STL at edges is fine and often clearer.
- Don't typedef things unless the alias carries real meaning (fixed-width `u32`/`f64` in `types.h` are the main exception).

## Naming

| Kind | Style | Example |
|------|-------|---------|
| Functions | PascalCase | `RunPipeline`, `FetchFromMock` |
| Variables | camelCase | `recordCount`, `maxNotional` |
| Types / enums | PascalCase | `BuyCandidates`, `FetchSource` |
| Constants | kCamelCase | `kMaxSymbols`, `kMaxPathLen` |
| Member variables | `m_` + camelCase | `m_startNs` |

## Documentation

- **Don't comment obvious code.** Simple loops and one-liners need no comment.
- Use `//` inline comments for non-obvious intent: pipeline stage boundaries, flag meanings, business rules.
- Command-line flags in `FLAGS(...)` blocks get a `//` doc line above each flag.
- Public API headers and important files (pipeline, core types) get a brief `///` file header.
- Functions only need `///` when behavior isn't obvious from the name.

## Errors and logging

- Every `return false` in production code must be preceded by `LOG_ERROR(...)` explaining what failed.
- Callers may return false without re-logging if the callee already logged the cause.
- Use `LOG(...)` / `LOG_ERROR(...)` from `ground/util/log.h` — not `printf` / `fprintf`.

## Formatting

- **4-space indentation**, **Allman braces** — see `.clang-format`
- Before pushing, run `sh format.sh` (or `sh format.sh --check` to verify without writing)

## Parameters

- Prefer references. Pointers only when nullable; null-check at entry.

## Pipeline data layout

- SoA with explicit `count` and fixed `kMax*` capacities in `core.h`.

## Abstractions

- No polymorphism or templates in pipeline code unless explicitly justified.
- `[[nodiscard]] bool` or `enum class` for errors; no exceptions in hot paths.

## Fetch config

`FetchConfig::source` selects which `FetchInput` member is active (`mock` vs `file.path`).
`file.path` is a `std::string` at the I/O boundary.

## Command-line flags

Each `FLAG` declares its variable at function scope — no separate declarations.

```cpp
FLAGS(
    // PRNG seed when using mock fetch.
    FLAG_DEFAULT(u32, seed, 42);
    // Output path for buy_candidates.bin.
    FLAG_DEFAULT_STR(exportPath, "buy_candidates.bin");
);
```

- Numeric: `FLAG(u32, name)`, `FLAG(u64, name)`, `FLAG_DEFAULT(u32, name, default)`
- String: `FLAG_STR(name)`, `FLAG_DEFAULT_STR(name, default)`

## File layout

- One `.h` + one `.cpp` per module where possible.
- Bazel: `BUILD.bazel`

## Bazel

- Dependency direction: `util` → `core` → stage libs → `pipeline` → process binaries.
