#!/bin/sh
# Format or check C++ sources under ground/ using .clang-format.
set -eu

cd "$(dirname "$0")"

file_count=$(find ground \( -name '*.cpp' -o -name '*.h' \) | wc -l)
if [ "$file_count" -eq 0 ]; then
    echo "No C++ files found under ground/" >&2
    exit 1
fi

if [ "${1:-}" = "--check" ]; then
    find ground \( -name '*.cpp' -o -name '*.h' \) | sort | xargs clang-format --dry-run --Werror
    echo "clang-format check passed ($file_count files)."
else
    find ground \( -name '*.cpp' -o -name '*.h' \) | sort | xargs clang-format -i
    echo "Formatted $file_count files."
fi
