#include <gtest/gtest.h>
#include "Ast.h"

using namespace Regex;

// ---------------------------------------------------------------------------------------------------------------------

class AstTest : public testing::Test
{
protected:
    AstTest() = default;
    ~AstTest() override = default;
};

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(AstTest, Empty)
{
    const auto empty = EmptyAst::Make();

    EXPECT_TRUE(empty->Matches(""));

    EXPECT_FALSE(empty->Matches("a"));
    EXPECT_FALSE(empty->Matches("b"));
    EXPECT_FALSE(empty->Matches("aa"));
    EXPECT_FALSE(empty->Matches("ab"));

    EXPECT_EQ(empty->ToString(), "");
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(AstTest, Symbol)
{
    const auto literal = SymbolAst::Make('a');

    EXPECT_TRUE(literal->Matches("a"));

    EXPECT_FALSE(literal->Matches(""));
    EXPECT_FALSE(literal->Matches("aa"));
    EXPECT_FALSE(literal->Matches("b"));

    EXPECT_EQ(literal->ToString(), "a");
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(AstTest, Alternation)
{
    auto pattern = AlternationAst::Make
    (
        AlternationAst::Make
        (
            EmptyAst::Make(),
            SymbolAst::Make('a')
        ),
        AlternationAst::Make
        (
            SymbolAst::Make('b'),
            AlternationAst::Make
            (
                SymbolAst::Make('c'),
                SymbolAst::Make('d')
            )
        )
    );

    EXPECT_TRUE(pattern->Matches(""));
    EXPECT_TRUE(pattern->Matches("a"));
    EXPECT_TRUE(pattern->Matches("b"));
    EXPECT_TRUE(pattern->Matches("c"));
    EXPECT_TRUE(pattern->Matches("d"));

    EXPECT_FALSE(pattern->Matches("aa"));
    EXPECT_FALSE(pattern->Matches("ab"));
    EXPECT_FALSE(pattern->Matches("ac"));
    EXPECT_FALSE(pattern->Matches("ad"));
    EXPECT_FALSE(pattern->Matches("bb"));
    EXPECT_FALSE(pattern->Matches("bc"));
    EXPECT_FALSE(pattern->Matches("bd"));
    EXPECT_FALSE(pattern->Matches("cc"));
    EXPECT_FALSE(pattern->Matches("cd"));
    EXPECT_FALSE(pattern->Matches("dd"));

    EXPECT_EQ(pattern->ToString(), "|a|b|c|d");
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(AstTest, Concatenation)
{
    auto pattern = ConcatenationAst::Make
    (
        AlternationAst::Make
        (
            SymbolAst::Make('a'),
            SymbolAst::Make('b')
        ),
        ConcatenationAst::Make
        (
            SymbolAst::Make('c'),
            SymbolAst::Make('d')
        )
    );

    EXPECT_TRUE(pattern->Matches("acd"));
    EXPECT_TRUE(pattern->Matches("bcd"));

    EXPECT_FALSE(pattern->Matches(""));
    EXPECT_FALSE(pattern->Matches("a"));
    EXPECT_FALSE(pattern->Matches("b"));
    EXPECT_FALSE(pattern->Matches("c"));
    EXPECT_FALSE(pattern->Matches("d"));
    EXPECT_FALSE(pattern->Matches("bc"));
    EXPECT_FALSE(pattern->Matches("cd"));
    EXPECT_FALSE(pattern->Matches("abc"));

    EXPECT_EQ(pattern->ToString(), "(a|b)cd");
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(AstTest, Repetition)
{
    auto pattern = RepetitionAst::Make
    (
        AlternationAst::Make
        (
            ConcatenationAst::Make
            (
                SymbolAst::Make('a'),
                SymbolAst::Make('b')
            ),
            SymbolAst::Make('c')
        )
    );

    EXPECT_TRUE(pattern->Matches(""));
    EXPECT_TRUE(pattern->Matches("c"));
    EXPECT_TRUE(pattern->Matches("ab"));
    EXPECT_TRUE(pattern->Matches("cc"));
    EXPECT_TRUE(pattern->Matches("abab"));
    EXPECT_TRUE(pattern->Matches("ccc"));
    EXPECT_TRUE(pattern->Matches("ababab"));

    EXPECT_FALSE(pattern->Matches("a"));
    EXPECT_FALSE(pattern->Matches("b"));
    EXPECT_FALSE(pattern->Matches("ac"));
    EXPECT_FALSE(pattern->Matches("bc"));

    EXPECT_EQ(pattern->ToString(), "(ab|c)*");
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(AstTest, OneOrMore)
{
    auto pattern = ConcatenationAst::Make
    (
        AlternationAst::Make
        (
            SymbolAst::Make('a'),
            OneOrMoreAst::Make
            (
                SymbolAst::Make('b')
            )
        ),
        SymbolAst::Make('c')
    );

    EXPECT_TRUE(pattern->Matches("ac"));
    EXPECT_TRUE(pattern->Matches("bc"));
    EXPECT_TRUE(pattern->Matches("bbc"));
    EXPECT_TRUE(pattern->Matches("bbbc"));

    EXPECT_FALSE(pattern->Matches("c"));
    EXPECT_FALSE(pattern->Matches("aac"));
    EXPECT_FALSE(pattern->Matches("abc"));
    EXPECT_FALSE(pattern->Matches("abbc"));

    EXPECT_EQ(pattern->ToString(), "(a|b+)c");
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(AstTest, Optional)
{
    auto pattern = ConcatenationAst::Make
    (
        AlternationAst::Make
        (
            OptionalAst::Make
            (
                SymbolAst::Make('a')
            ),
            SymbolAst::Make('b')
        ),
        SymbolAst::Make('c')
    );

    EXPECT_TRUE(pattern->Matches("c"));
    EXPECT_TRUE(pattern->Matches("ac"));
    EXPECT_TRUE(pattern->Matches("bc"));

    EXPECT_FALSE(pattern->Matches("aac"));
    EXPECT_FALSE(pattern->Matches("abc"));

    EXPECT_EQ(pattern->ToString(), "(a?|b)c");
}