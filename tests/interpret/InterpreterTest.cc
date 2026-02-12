#include <interpret/Interpreter.h>
#include <interpret/Runner.h>
#include <interpret/Utils/PrintVisitor.h>

#include <regex>
#include <gtest/gtest.h>

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------

class InterpreterTest : public testing::Test
{
protected:
    InterpreterTest() = default;
    ~InterpreterTest() override = default;
};

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(InterpreterTest, ForStatement)
{
    auto result = Runner::Run(
        "var a; for (a = 0; a < 5; a = a + 1) {} a;"
    );
    EXPECT_EQ(result, 5);

    result = Runner::Run(
        "var a; for (a = 0; a < 5; a = a + 1) { print a; } a / 2;"
    );
    EXPECT_EQ(result, 2.5);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(InterpreterTest, WhileStatement)
{
    auto result = Runner::Run(
        "var a = 0; while (a < 5) { a = a + 1; } a;"
    );
    EXPECT_EQ(result, 5);

    result = Runner::Run(
        "var a = 0; var b = 3; while(a < 3 and b > 0) { a = a + 1; b = b - 1; } a == 3 and b == 0;"
    );
    EXPECT_EQ(result, true);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(InterpreterTest, IfStatement)
{
    auto result = Runner::Run(
        "var x = 1; var y = 2; if (x == 1) { y = x + y - 1; } else { y = x; } y;"
    );
    EXPECT_EQ(result, 2);

    result = Runner::Run(
        "var x = 2; var y; if (x != 2) { y = x; } else { y = x * -1 + 5; } y;"
    );
    EXPECT_EQ(result, 3);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(InterpreterTest, VarAssignment)
{
    auto result = Runner::Run(
        "{ "
        "  var x = 3; var y = x * 2 - 1; var a = x + y;"
        "  {"
        "    x = 9; y = 20; var z = x + y + 1; a = a + z;"
        "  }"
        "  a;"
        "}"
    );
    EXPECT_EQ(result, 38);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(InterpreterTest, Blocks)
{
    Runner::Reset();

    auto result = Runner::Run(
        "var z; var x = 11; var y = 88;"
        "{"
        "  var x = 2; var y = \"test\"; print x; print y;"
        "  {"
        "      x = 3; y = 20; print x; print y;"
        "  }"
        "  print x; print y; "
        "}"
        "print x; print y; z = x + y;"
    );

    EXPECT_EQ(result, 99);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(InterpreterTest, StringConcat)
{
    const static std::string kSource = "var str = \"test\"; str = \"concat_\" + str;";

    Runner::Reset();

    auto result = Runner::Run(kSource);
    EXPECT_EQ(result, "concat_test");
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(InterpreterTest, Function)
{
    Runner::Reset();

    auto result = Runner::Run("func Sum(a,b) { return a + b; }");
    result = Runner::Run("var t = Sum(1,2) + Sum(Sum(1, 2), Sum(-1,1));");

    EXPECT_EQ(result, 6);
}

// ---------------------------------------------------------------------------------------------------------------------

TEST_F(InterpreterTest, Arithmetic)
{
    auto result = Runner::Run("10 + 8 + (- 3);");
    EXPECT_EQ(result, 15);

    result = Runner::Run("(5 < 6) + 7 * -2;");
    EXPECT_EQ(result, -13);

    result = Runner::Run("5.0 / 2.5 - 3.0 * 0.5;");
    EXPECT_EQ(result, 0.5);
}