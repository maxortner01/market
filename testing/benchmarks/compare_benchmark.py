#!/usr/bin/env python3
"""Compare Google Benchmark JSON output against a simple baseline file."""

import json
import sys


def load_json(path):
    with open(path, "r", encoding="utf-8") as handle:
        return json.load(handle)


def to_nanoseconds(value, time_unit):
    if time_unit == "ns":
        return float(value)
    if time_unit == "us":
        return float(value) * 1_000.0
    if time_unit == "ms":
        return float(value) * 1_000_000.0
    if time_unit == "s":
        return float(value) * 1_000_000_000.0
    raise ValueError(f"unsupported time unit: {time_unit}")


def main():
    if len(sys.argv) != 3:
        print("usage: compare_benchmark.py <results.json> <baseline.json>", file=sys.stderr)
        return 2

    results = load_json(sys.argv[1])
    baseline = load_json(sys.argv[2])

    thresholds = {
        entry["name"]: entry["real_time_threshold_ns"] for entry in baseline.get("benchmarks", [])
    }

    failures = []
    for bench in results.get("benchmarks", []):
        name = bench.get("name", "")
        if name not in thresholds:
            continue

        time_unit = bench.get("time_unit", "ns")
        real_time_ns = to_nanoseconds(bench.get("real_time", 0.0), time_unit)
        threshold_ns = thresholds[name]
        if real_time_ns > threshold_ns * 1.05:
            failures.append(
                f"{name}: {real_time_ns:.0f}ns exceeds baseline threshold {threshold_ns}ns (+5%)"
            )

    if failures:
        print("Benchmark regressions detected:", file=sys.stderr)
        for failure in failures:
            print(failure, file=sys.stderr)
        return 1

    print("Benchmark comparison passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
