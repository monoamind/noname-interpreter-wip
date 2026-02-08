#include <interpret/Interpreter.h>
#include <interpret/Runner.h>
#include <interpret/Utils/PrintVisitor.h>

#include <regex>
#include <gtest/gtest.h>

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------

class CompilerParserTest : public testing::Test
{
protected:
    CompilerParserTest() = default;
    ~CompilerParserTest() override = default;
};

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(CompilerParserTest, ExprPlus)
{
    const static std::string kSource = "10 + 8 + (- 3)";

    Scanner scanner(kSource);
    Parser parser(scanner.ScanTokens());
    auto astRoot = parser.Parse();

    Interpreter inter;
    inter.Interpret(*astRoot);

    EXPECT_EQ(inter.Result(), 15);

    std::cout << inter.Result() << "\n";
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(CompilerParserTest, ExprPull)
{
    const static std::string kSource = "(5 < 6) + 7 * -2;";

    Scanner scanner(kSource);
    Parser parser(scanner.ScanTokens());
    auto astRoot = parser.Parse();

    Interpreter inter;
    inter.Interpret(*astRoot);

    EXPECT_EQ(inter.Result(), -13);

    std::cout << inter.Result() << "\n";
}

TEST_F(CompilerParserTest, Parser2)
{
    const static std::string kSource = "5 - 3 * 2;";

    Scanner scanner(kSource);
    Parser parser(scanner.ScanTokens());
    auto astRoot = parser.Parse();

    Interpreter inter;
    inter.Interpret(*astRoot);

    EXPECT_EQ(inter.Result(), -1);

    std::cout << inter.Result() << "\n";
}

TEST_F(CompilerParserTest, Parser3)
{
    const static std::string kSource = "5.0 / 2.5 - 3.0 * 0.5;";

    Scanner scanner(kSource);
    Parser parser(scanner.ScanTokens());
    auto astRoot = parser.Parse();

    Interpreter inter;
    inter.Interpret(*astRoot);

    EXPECT_EQ(inter.Result(), 0.5);

    std::cout << inter.Result() << "\n";
}

// ---------------------------------------------------------------------------------------------------------------------