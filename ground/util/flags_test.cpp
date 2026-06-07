#include "ground/util/flags.h"

#include <gtest/gtest.h>

/// Verifies --name=value overrides are parsed into registered variables.
TEST(FlagsTest, ParsesU32AndStringFlags)
{
    char arg0[] = "test";
    char arg1[] = "--seed=99";
    char arg2[] = "--exportPath=/tmp/out.bin";
    char* argv[] = {arg0, arg1, arg2};
    int argc = 3;

    u32 seed = 0;
    std::string exportPath;

    {
        ground::FlagParser parser(argc, argv);
        parser.U32("seed", seed, 42);
        parser.String("exportPath", exportPath, "buy_candidates.bin");
        ASSERT_TRUE(parser.Parse());
    }

    EXPECT_EQ(seed, 99u);
    EXPECT_EQ(exportPath, "/tmp/out.bin");
}

/// Verifies registered defaults apply when argv omits those flags.
TEST(FlagsTest, AppliesDefaultsWhenFlagsMissing)
{
    char arg0[] = "test";
    char* argv[] = {arg0};
    int argc = 1;

    u32 seed = 0;
    std::string exportPath;

    {
        ground::FlagParser parser(argc, argv);
        parser.U32("seed", seed, 42);
        parser.String("exportPath", exportPath, "buy_candidates.bin");
        ASSERT_TRUE(parser.Parse());
    }

    EXPECT_EQ(seed, 42u);
    EXPECT_EQ(exportPath, "buy_candidates.bin");
}
