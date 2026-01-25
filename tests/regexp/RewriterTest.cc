#include <gtest/gtest.h>

#include "Parser.h"
#include "Rewriter.h"
#include "Scanner.h"

using namespace Regex;

// ---------------------------------------------------------------------------------------------------------------------

class RewriterTest : public testing::Test
{};

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RewriterTest, PatternA)
{
    Rewriter rwr{ R"(\[\]ABCD\\)" };
    auto expect = R"([]ABCD\)";

    EXPECT_EQ(rwr.Result(), expect);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RewriterTest, PatternB)
{
    Rewriter rwr{ R"([\[\]]AB[\[-\]])" };
    auto expect = R"(([|])AB([|\|]))";

    EXPECT_EQ(rwr.Result(), expect);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RewriterTest, PatternC)
{
    Rewriter rwr{ R"([A-Ba-a\-\\]?)" };
    auto expect = R"((A|B|a|-|\)?)";

    EXPECT_EQ(rwr.Result(), expect);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RewriterTest, PatternD)
{
    Rewriter rwr{ R"([\--?\@ABC\^]?)" };
    auto expect = R"((-|.|/|0|1|2|3|4|5|6|7|8|9|:|;|<|=|>|?|@|A|B|C|^)?)";

    EXPECT_EQ(rwr.Result(), expect);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RewriterTest, PatternE)
{
    Rewriter rwr{ "A[1-9]+[B_0]" };
    auto expect = "A(1|2|3|4|5|6|7|8|9)+(B|_|0)";

    EXPECT_EQ(rwr.Result(), expect);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RewriterTest, PatternF)
{
    Rewriter rwr{ "[5-8]a+[0-3]?" };
    auto expect = "(5|6|7|8)a+(0|1|2|3)?";

    EXPECT_EQ(rwr.Result(), expect);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RewriterTest, PatternG)
{
    Rewriter rwr{ R"([Q-b]+[0-3A\-]?)" };
    auto expect = R"((Q|R|S|T|U|V|W|X|Y|Z|[|\|]|^|_|`|a|b)+(0|1|2|3|A|-)?)";

    EXPECT_EQ(rwr.Result(), expect);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RewriterTest, PatternH)
{
    Rewriter rw1{ R"(\w)" };
    Rewriter rw2{ R"([A-Za-z0-9_])" };

    EXPECT_EQ(rw1.Result(), rw2.Result());
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RewriterTest, PatternI)
{
    Rewriter rw1{ R"(\d)" };
    Rewriter rw2{ R"([0-9])" };

    EXPECT_EQ(rw1.Result(), rw2.Result());
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RewriterTest, PatternJ)
{
    Rewriter rw1{ R"(\s)" };
    Rewriter rw2{ R"([ \n\r\t\f\v])" };

    EXPECT_EQ(rw1.Result(), rw2.Result());
}