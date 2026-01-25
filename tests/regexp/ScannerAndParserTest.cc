#include <gtest/gtest.h>
#include "Parser.h"
#include "Scanner.h"

using namespace Regex;

// ---------------------------------------------------------------------------------------------------------------------

class ScannerAndParserTest : public testing::Test
{};

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(ScannerAndParserTest, PatternA)
{
    Scanner scanner("");
    const auto& tokens = scanner.ScanTokens();

    EXPECT_TRUE(tokens.size() == 1);
    EXPECT_TRUE(tokens[0].IsEndToken());
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(ScannerAndParserTest, PatternAB)
{
    Scanner scanner("a*b*");
    const auto& tokens = scanner.ScanTokens();

    Parser parser(tokens);
    auto parseTreeRoot = parser.Parse();
    auto reNode = parseTreeRoot->ConvertToAst();

    EXPECT_TRUE(reNode->ToString() == "a*b*");
}

TEST_F(ScannerAndParserTest, PatternB)
{
    Scanner scanner("a+b?");
    const auto& tokens = scanner.ScanTokens();

    Parser parser(tokens);
    auto parseTreeRoot = parser.Parse();
    auto reNode = parseTreeRoot->ConvertToAst();

    EXPECT_TRUE(reNode->ToString() == "a+b?");
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(ScannerAndParserTest, PatternC)
{
    Scanner scanner("(a|b)*c");
    const auto& tokens = scanner.ScanTokens();

    Parser parser(tokens);
    auto parseTreeRoot = parser.Parse();
    auto reNode = parseTreeRoot->ConvertToAst();

    EXPECT_TRUE(reNode->ToString() == "(a|b)*c");
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(ScannerAndParserTest, PatternD)
{
    Scanner scanner("(ab|a)*");
    const auto& tokens = scanner.ScanTokens();

    Parser parser(tokens);
    auto parseTreeRoot = parser.Parse();
    auto reNode = parseTreeRoot->ConvertToAst();

    EXPECT_TRUE(reNode->ToString() == "(ab|a)*");
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(ScannerAndParserTest, PatternE)
{
    Scanner scanner("(|b)*");
    const auto& tokens = scanner.ScanTokens();

    Parser parser(tokens);
    auto parseTreeRoot = parser.Parse();
    auto reNode = parseTreeRoot->ConvertToAst();

    EXPECT_TRUE(reNode->ToString() == "(|b)*");
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(ScannerAndParserTest, PatternF)
{
    Scanner scanner("(a(|b))*");
    const auto& tokens = scanner.ScanTokens();

    Parser parser(tokens);
    auto parseTreeRoot = parser.Parse();
    auto reNode = parseTreeRoot->ConvertToAst();

    EXPECT_TRUE(reNode->ToString() == "(a(|b))*");
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(ScannerAndParserTest, PatternG)
{
    Scanner scanner("(a(c|b))*");
    const auto& tokens = scanner.ScanTokens();

    Parser parser(tokens);
    auto parseTreeRoot = parser.Parse();
    auto reNode = parseTreeRoot->ConvertToAst();

    EXPECT_TRUE(reNode->ToString() == "(a(c|b))*");
}