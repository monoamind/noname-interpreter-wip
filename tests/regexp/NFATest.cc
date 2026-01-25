#include <gtest/gtest.h>
#include "NFA.h"

using namespace Regex;

// ---------------------------------------------------------------------------------------------------------------------

class NFATest : public testing::Test
{
protected:
    NFATest()
    {
        nfaA_ = NFA(4);
        nfaA_.ResetFinalStates({ 3 });
        nfaA_.AddTransition(0, 'a', 0);
        nfaA_.AddTransition(0, 'b', 0);
        nfaA_.AddTransition(0, 'b', 1);
        nfaA_.AddTransition(1, 'a', 2);
        nfaA_.AddTransition(1, 'b', 2);
        nfaA_.AddTransition(2, 'a', 3);
        nfaA_.AddTransition(2, 'b', 3);

        nfaB_ = NFA(6);
        nfaB_.ResetFinalStates({ 1, 3 });
        nfaB_.AddTransition(0, {}, 1);
        nfaB_.AddTransition(0, {}, 3);
        nfaB_.AddTransition(1, 'a', 2);
        nfaB_.AddTransition(2, 'a', 1);
        nfaB_.AddTransition(3, 'a', 4);
        nfaB_.AddTransition(4, 'a', 5);
        nfaB_.AddTransition(5, 'a', 3);
    }

    ~NFATest() override = default;

protected:
    NFA nfaA_;
    NFA nfaB_;
};

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(NFATest, NoFreeMoves)
{
    EXPECT_TRUE(nfaA_.Accepts("baa"));
    EXPECT_TRUE(nfaA_.Accepts("bab"));
    EXPECT_TRUE(nfaA_.Accepts("bba"));
    EXPECT_TRUE(nfaA_.Accepts("bbb"));
    EXPECT_TRUE(nfaA_.Accepts("abaa"));
    EXPECT_TRUE(nfaA_.Accepts("abab"));
    EXPECT_TRUE(nfaA_.Accepts("abba"));
    EXPECT_TRUE(nfaA_.Accepts("bbaa"));
    EXPECT_TRUE(nfaA_.Accepts("bbab"));
    EXPECT_TRUE(nfaA_.Accepts("bbbbb"));
    EXPECT_TRUE(nfaA_.Accepts("ababababbb"));

    EXPECT_FALSE(nfaA_.Accepts(""));
    EXPECT_FALSE(nfaA_.Accepts("a"));
    EXPECT_FALSE(nfaA_.Accepts("b"));
    EXPECT_FALSE(nfaA_.Accepts("ab"));
    EXPECT_FALSE(nfaA_.Accepts("bb"));
    EXPECT_FALSE(nfaA_.Accepts("abb"));
    EXPECT_FALSE(nfaA_.Accepts("babb"));
    EXPECT_FALSE(nfaA_.Accepts("bbabb"));
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(NFATest, HasFreeMovesAndAcceptsString)
{
    EXPECT_TRUE(nfaB_.Accepts(""));
    EXPECT_TRUE(nfaB_.Accepts("aa"));
    EXPECT_TRUE(nfaB_.Accepts("aaa"));
    EXPECT_TRUE(nfaB_.Accepts("aaaa"));
    EXPECT_TRUE(nfaB_.Accepts("aaaaaaaaa"));

    EXPECT_FALSE(nfaB_.Accepts("a"));
    EXPECT_FALSE(nfaB_.Accepts("aaaaa"));
    EXPECT_FALSE(nfaB_.Accepts("aaaaaaa"));
}