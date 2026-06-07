#include "ground/util/flags.h"

#include "ground/util/log.h"

#include <cstring>

namespace ground
{

FlagParser::FlagParser(int argc, char **argv) : m_argc(argc), m_argv(argv), m_entryCount(0) {}

FlagParser &FlagParser::U64(const char *name, u64 &value, u64 defaultValue)
{
    if (m_entryCount >= kMaxFlags || name == nullptr)
    {
        LOG_ERROR("FlagParser: failed to register u64 flag");
        return *this;
    }

    value = defaultValue;
    m_entries[m_entryCount] = {};
    m_entries[m_entryCount].kind = Kind::U64;
    m_entries[m_entryCount].name = name;
    m_entries[m_entryCount].u64Value = &value;
    m_entries[m_entryCount].u64Default = defaultValue;
    ++m_entryCount;
    return *this;
}

FlagParser &FlagParser::U32(const char *name, u32 &value, u32 defaultValue)
{
    if (m_entryCount >= kMaxFlags || name == nullptr)
    {
        LOG_ERROR("FlagParser: failed to register u32 flag");
        return *this;
    }

    value = defaultValue;
    m_entries[m_entryCount] = {};
    m_entries[m_entryCount].kind = Kind::U32;
    m_entries[m_entryCount].name = name;
    m_entries[m_entryCount].u32Value = &value;
    m_entries[m_entryCount].u32Default = defaultValue;
    ++m_entryCount;
    return *this;
}

FlagParser &FlagParser::String(const char *name, std::string &value,
                               const std::string &defaultValue)
{
    if (m_entryCount >= kMaxFlags || name == nullptr)
    {
        LOG_ERROR("FlagParser: failed to register string flag");
        return *this;
    }

    value = defaultValue;
    m_entries[m_entryCount] = {};
    m_entries[m_entryCount].kind = Kind::String;
    m_entries[m_entryCount].name = name;
    m_entries[m_entryCount].stringValue = &value;
    m_entries[m_entryCount].stringDefault = defaultValue;
    ++m_entryCount;
    return *this;
}

bool FlagParser::Parse()
{
    // Seed every registered flag with its default before applying argv overrides.
    for (u32 i = 0; i < m_entryCount; ++i)
    {
        if (m_entries[i].kind == Kind::U32 && m_entries[i].u32Value != nullptr)
        {
            *m_entries[i].u32Value = m_entries[i].u32Default;
        }
        else if (m_entries[i].kind == Kind::U64 && m_entries[i].u64Value != nullptr)
        {
            *m_entries[i].u64Value = m_entries[i].u64Default;
        }
        else if (m_entries[i].kind == Kind::String && m_entries[i].stringValue != nullptr)
        {
            *m_entries[i].stringValue = m_entries[i].stringDefault;
        }
    }

    for (int argIndex = 1; argIndex < m_argc; ++argIndex)
    {
        const char *arg = m_argv[argIndex];
        if (arg == nullptr || arg[0] != '-' || arg[1] != '-')
        {
            continue;
        }

        const char *flagBody = arg + 2;
        const char *equalSign = std::strchr(flagBody, '=');
        std::string flagName;
        const char *flagValue = nullptr;

        if (equalSign != nullptr)
        {
            flagName.assign(flagBody, static_cast<std::size_t>(equalSign - flagBody));
            flagValue = equalSign + 1;
        }
        else
        {
            flagName = flagBody;
            if (argIndex + 1 < m_argc && m_argv[argIndex + 1] != nullptr &&
                m_argv[argIndex + 1][0] != '-')
            {
                flagValue = m_argv[argIndex + 1];
                ++argIndex;
            }
        }

        if (flagValue == nullptr)
        {
            LOG_ERROR("flag --%s requires a value", flagName.c_str());
            return false;
        }

        bool matched = false;
        for (u32 i = 0; i < m_entryCount; ++i)
        {
            if (m_entries[i].name == nullptr || flagName != m_entries[i].name)
            {
                continue;
            }

            matched = true;
            if (m_entries[i].kind == Kind::U32 && m_entries[i].u32Value != nullptr)
            {
                *m_entries[i].u32Value = static_cast<u32>(std::strtoul(flagValue, nullptr, 10));
            }
            else if (m_entries[i].kind == Kind::U64 && m_entries[i].u64Value != nullptr)
            {
                *m_entries[i].u64Value = static_cast<u64>(std::strtoull(flagValue, nullptr, 10));
            }
            else if (m_entries[i].kind == Kind::String && m_entries[i].stringValue != nullptr)
            {
                *m_entries[i].stringValue = flagValue;
            }
            break;
        }

        if (!matched)
        {
            LOG_ERROR("unknown flag --%s", flagName.c_str());
            return false;
        }
    }

    return true;
}

} // namespace ground
