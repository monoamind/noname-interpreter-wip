#include <gtest/gtest.h>
#include "RegExp.h"
#include "Parser.h"
#include "Scanner.h"

using namespace Regex;

// ---------------------------------------------------------------------------------------------------------------------

class RegExpSearchTest : public testing::Test
{
protected:
    using MatchList = std::invoke_result_t<decltype(&RegExp::FindMatches), RegExp, std::string>;
    using Match = MatchList::value_type;

protected:
    RegExp reA_{"(ab|a)*"};
    RegExp reB_{"(a(|b))*"};
    RegExp reC_{"(ab|a)*"};
    RegExp reD_{"(ab|cd)*"};
};

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RegExpSearchTest, EmptyTest)
{
    using enum TokenType;

    Scanner scanner("");
    const auto& tokens = scanner.ScanTokens();

    EXPECT_TRUE(tokens.size() == 1);
    EXPECT_TRUE(tokens[0].IsEndToken());
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RegExpSearchTest, RegExpA_FindMatches)
{
    // "(ab|a)*"

    EXPECT_TRUE(reA_.Matches(""));

    auto matchesA = reA_.FindMatches("abaabaaba");
    {
        EXPECT_TRUE(matchesA.size() == 1);
        EXPECT_TRUE(matchesA[0] == Match({0, "abaabaaba"}));
    }

    auto matchesB = reA_.FindMatches("cabaabaabac");
    {
        EXPECT_TRUE(matchesB.size() == 1);
        EXPECT_TRUE(matchesB[0] == Match({1, "abaabaaba"}));
    }

    auto matchesC = reA_.FindMatches("cabacabaabca");
    {
        EXPECT_TRUE(matchesC.size() == 3);

        EXPECT_TRUE(matchesC[0] == Match({1, "aba"}));
        EXPECT_TRUE(matchesC[1] == Match({5, "abaab"}));
        EXPECT_TRUE(matchesC[2] == Match({11, "a"}));
    }

    auto matchesD = reA_.FindMatches("cabaabaaba");
    {
        EXPECT_TRUE(matchesD.size() == 1);

        EXPECT_TRUE(matchesD[0].first == 1);
        EXPECT_TRUE(matchesD[0].second == "abaabaaba");
    }
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RegExpSearchTest, RegExpA_Matches)
{
    // "(ab|a)*"

    EXPECT_TRUE(reA_.Matches(""));

    EXPECT_TRUE(reA_.Matches("a"));
    EXPECT_TRUE(reA_.Matches("aa"));
    EXPECT_TRUE(reA_.Matches("aaa"));

    EXPECT_TRUE(reA_.Matches("ab"));
    EXPECT_TRUE(reA_.Matches("abab"));
    EXPECT_TRUE(reA_.Matches("ababab"));

    EXPECT_TRUE(reA_.Matches("aab"));
    EXPECT_TRUE(reA_.Matches("aba"));

    EXPECT_TRUE(reA_.Matches("aaba"));
    EXPECT_TRUE(reA_.Matches("abaab"));

    EXPECT_FALSE(reA_.Matches("abb"));
    EXPECT_FALSE(reA_.Matches("abbb"));
    EXPECT_FALSE(reA_.Matches("b"));
    EXPECT_FALSE(reA_.Matches("ba"));
    EXPECT_FALSE(reA_.Matches("bab"));
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RegExpSearchTest, RegExpB_FindMatches)
{
    // "(a(|b))*"

    EXPECT_TRUE(reB_.Matches(""));

    auto matchesA = reB_.FindMatches("abababab");
    {
        EXPECT_TRUE(matchesA.size() == 1);

        EXPECT_TRUE(matchesA[0].first == 0);
        EXPECT_TRUE(matchesA[0].second == "abababab");
    }

    auto matchesB = reB_.FindMatches("abaabaabac");
    {
        EXPECT_TRUE(matchesB.size() == 1);

        EXPECT_TRUE(matchesB[0].first == 0);
        EXPECT_TRUE(matchesB[0].second == "abaabaaba");
    }

    auto matchesC = reA_.FindMatches("cabacabaabca");
    {
        EXPECT_TRUE(matchesC.size() == 3);

        EXPECT_TRUE(matchesC[0].first == 1);
        EXPECT_TRUE(matchesC[0].second == "aba");

        EXPECT_TRUE(matchesC[1].first == 5);
        EXPECT_TRUE(matchesC[1].second == "abaab");

        EXPECT_TRUE(matchesC[2].first == 11);
        EXPECT_TRUE(matchesC[2].second == "a");
    }

    auto matchesD = reA_.FindMatches("cabaabaaba");
    {
        EXPECT_TRUE(matchesD.size() == 1);

        EXPECT_TRUE(matchesD[0].first == 1);
        EXPECT_TRUE(matchesD[0].second == "abaabaaba");
    }
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RegExpSearchTest, RegExpC_FindMatches)
{
    RegExp reC("abcd");

    EXPECT_TRUE(reC.Matches("abcd"));
    EXPECT_FALSE(reC.Matches("abcabcabd"));

    auto matchesA = reC.FindMatches("abcd");
    {
        EXPECT_TRUE(matchesA.size() == 1);

        EXPECT_TRUE(matchesA[0].first == 0);
        EXPECT_TRUE(matchesA[0].second == "abcd");
    }

    auto matchesB = reC.FindMatches("abcdxabcd");
    {
        EXPECT_TRUE(matchesB.size() == 2);

        EXPECT_TRUE(matchesB[0].first == 0);
        EXPECT_TRUE(matchesB[0].second == "abcd");
    }

    auto matchesC = reC.FindMatches("abcdabcd");
    {
        EXPECT_TRUE(matchesC.size() == 2);

        EXPECT_TRUE(matchesC[0].first == 0);
        EXPECT_TRUE(matchesC[0].second == "abcd");

        EXPECT_TRUE(matchesC[1].first == 4);
        EXPECT_TRUE(matchesC[1].second == "abcd");
    }

    auto matchesD = reC.FindMatches("xabcdabcdx");
    {
        EXPECT_TRUE(matchesD.size() == 2);

        EXPECT_TRUE(matchesD[0].first == 1);
        EXPECT_TRUE(matchesD[0].second == "abcd");

        EXPECT_TRUE(matchesD[1].first == 5);
        EXPECT_TRUE(matchesD[1].second == "abcd");
    }

    auto matchesE = reC.FindMatches("abcabcabd");
    {
        EXPECT_TRUE(matchesE.size() == 0);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(RegExpSearchTest, RegExpD_FindMatches)
{
    RegExp reD("abcd|xyz");

    EXPECT_TRUE(reD.Matches("abcd"));
    EXPECT_TRUE(reD.Matches("xyz"));

    EXPECT_FALSE(reD.Matches("abcdxyz"));
    EXPECT_FALSE(reD.Matches("abc"));
    EXPECT_FALSE(reD.Matches("ayz"));
    EXPECT_FALSE(reD.Matches("xy"));
    EXPECT_FALSE(reD.Matches("xbcd"));

    Match x = std::pair(0, "ata");
    auto matchesA1 = reD.FindMatches("abcd");
    {
        EXPECT_TRUE(matchesA1.size() == 1);

        EXPECT_TRUE(matchesA1[0].first == 0);
        EXPECT_TRUE(matchesA1[0].second == "abcd");
    }

    auto matchesA2 = reD.FindMatches("xyz");
    {
        EXPECT_TRUE(matchesA2.size() == 1);

        EXPECT_TRUE(matchesA2[0].first == 0);
        EXPECT_TRUE(matchesA2[0].second == "xyz");
    }

    auto matchesB = reD.FindMatches("abcdxxyz");
    {
        EXPECT_TRUE(matchesB.size() == 2);


        EXPECT_TRUE(matchesB[0] == Match({0, "abcd"}));
        EXPECT_TRUE(matchesB[1] == Match({5, "xyz"}));
    }

    auto matchesC = reD.FindMatches("abcdabcd");
    {
        EXPECT_TRUE(matchesC.size() == 2);

        EXPECT_TRUE(matchesC[0].first == 0);
        EXPECT_TRUE(matchesC[0].second == "abcd");

        EXPECT_TRUE(matchesC[1].first == 4);
        EXPECT_TRUE(matchesC[1].second == "abcd");
    }

    auto matchesD = reD.FindMatches("xabcdabcdx");
    {
        EXPECT_TRUE(matchesD.size() == 2);

        EXPECT_TRUE(matchesD[0].first == 1);
        EXPECT_TRUE(matchesD[0].second == "abcd");

        EXPECT_TRUE(matchesD[1].first == 5);
        EXPECT_TRUE(matchesD[1].second == "abcd");
    }

    auto matchesE = reD.FindMatches("abcabcabd");
    {
        EXPECT_TRUE(matchesE.size() == 0);
    }
}