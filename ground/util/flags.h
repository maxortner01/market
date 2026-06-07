/// Command-line flag parsing for process entry points.
#pragma once

#include "ground/util/types.h"

#include <string>

namespace ground
{

/// Parses --name=value flags from argv into caller-owned variables.
class FlagParser
{
  public:
    /// Captures argc/argv for a later Parse() call.
    explicit FlagParser(int argc, char** argv);

    /// Registers a u64 flag; returns *this for chaining.
    FlagParser& U64(const char* name, u64& value, u64 defaultValue = 0);
    /// Registers a u32 flag; returns *this for chaining.
    FlagParser& U32(const char* name, u32& value, u32 defaultValue = 0);
    /// Registers a string flag; returns *this for chaining.
    FlagParser& String(const char* name, std::string& value, const std::string& defaultValue = "");

    /// Applies defaults then overrides from argv; false on unknown or malformed flags.
    [[nodiscard]] bool Parse();

  private:
    /// Hard cap on registered flags per parser instance.
    static constexpr u32 kMaxFlags = 32;

    /// Storage kind for a registered flag entry.
    enum class Kind : u8
    {
        U32,
        U64,
        String,
    };

    /// Internal registry row linking a flag name to its destination variable.
    struct Entry
    {
        /// Which scalar type this row holds.
        Kind kind;
        /// Long option name without leading dashes.
        const char* name;
        /// Destination for u32 values (null when kind != U32).
        u32* u32Value;
        /// Default applied before argv overrides.
        u32 u32Default;
        /// Destination for u64 values (null when kind != U64).
        u64* u64Value;
        /// Default applied before argv overrides.
        u64 u64Default;
        /// Destination for string values (null when kind != String).
        std::string* stringValue;
        /// Default applied before argv overrides.
        std::string stringDefault;
    };

    /// Argument count from main().
    int m_argc;
    /// Argument vector from main().
    char** m_argv;
    /// Registered flag table.
    Entry m_entries[kMaxFlags];
    /// Number of rows populated in m_entries.
    u32 m_entryCount;
};

} // namespace ground

/// Declares flag variables at function scope, registers them, and parses argv.
#define FLAGS(...)                                                                                 \
    ground::FlagParser parser(argc, argv);                                                         \
    __VA_ARGS__                                                                                    \
    if (!parser.Parse())                                                                           \
    {                                                                                              \
        return 1;                                                                                  \
    }

#define FLAG_NARG(...) FLAG_NARG_(__VA_ARGS__, 4, 3, 2, 1)
#define FLAG_NARG_(_1, _2, _3, _4, N, ...) N
#define FLAG_CAT(a, b) FLAG_CAT2(a, b)
#define FLAG_CAT2(a, b) a##b
#define FLAG(...) FLAG_CAT(FLAG_, FLAG_NARG(__VA_ARGS__))(__VA_ARGS__)
#define FLAG_2(type, name) FLAG_IMPL_##type(name)

#define FLAG_DEFAULT(...) FLAG_CAT(FLAG_DEFAULT_, FLAG_DEFAULT_NARG(__VA_ARGS__))(__VA_ARGS__)
#define FLAG_DEFAULT_NARG(...) FLAG_DEFAULT_NARG_(__VA_ARGS__, 5, 4, 3, 2, 1)
#define FLAG_DEFAULT_NARG_(_1, _2, _3, _4, _5, N, ...) N
#define FLAG_DEFAULT_3(type, name, defaultValue) FLAG_DEFAULT_IMPL_##type(name, defaultValue)

#define FLAG_IMPL_u64(name)                                                                        \
    u64 name = 0;                                                                                  \
    parser.U64(#name, name)

#define FLAG_DEFAULT_IMPL_u64(name, defaultValue)                                                  \
    u64 name = defaultValue;                                                                       \
    parser.U64(#name, name, defaultValue)

#define FLAG_IMPL_u32(name)                                                                        \
    u32 name = 0;                                                                                  \
    parser.U32(#name, name)

#define FLAG_DEFAULT_IMPL_u32(name, defaultValue)                                                  \
    u32 name = defaultValue;                                                                       \
    parser.U32(#name, name, defaultValue)

#define FLAG_STR(name)                                                                             \
    std::string name;                                                                              \
    parser.String(#name, name)

#define FLAG_DEFAULT_STR(name, defaultValue)                                                       \
    std::string name = defaultValue;                                                               \
    parser.String(#name, name, defaultValue)
