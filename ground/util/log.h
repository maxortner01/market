/// Logging macros for stdout/stderr output.
#pragma once

#include <cstdio>

/// Log an informational message to stdout.
#define LOG(fmt, ...) std::fprintf(stdout, fmt "\n", ##__VA_ARGS__)

/// Log an error message to stderr.
#define LOG_ERROR(fmt, ...) std::fprintf(stderr, fmt "\n", ##__VA_ARGS__)
