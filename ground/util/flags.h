/// Command-line flag parsing for process entry points.
#pragma once

#include "ground/util/types.h"

#include <string>

namespace ground
{

class FlagParser
{
  public:
    explicit FlagParser(int argc, char **argv);

    FlagParser &U64(const char *name, u64 &value, u64 defaultValue = 0);
    FlagParser &U32(const char *name, u32 &value, u32 defaultValue = 0);
    FlagParser &String(const char *name, std::string &value, const std::string &defaultValue = "");

    [[nodiscard]] bool Parse();

  private:
    static constexpr u32 kMaxFlags = 32;

    enum class Kind : u8
    {
        U32,
        U64,
        String,
    };

    struct Entry
    {
        Kind kind;
        const char *name;
        u32 *u32Value;
        u32 u32Default;
        u64 *u64Value;
        u64 u64Default;
        std::string *stringValue;
        std::string stringDefault;
    };

    int m_argc;
    char **m_argv;
    Entry m_entries[kMaxFlags];
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
