#include "ExpressionAst.h"
#include "Parser.h"

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
// <expression> ::= <assignment>
// <assignment> ::= <variable_expr> "=" <expression>
// <or>         ::= <and> ("or" <and>)*
// <and>        ::= <equality> ("and" <equality>)*
// <equality>   ::= <comparison> (("==" | "!=") <comparison>)*
// <comparison> ::= <term> ((">" | ">=" | "<" | "<=") <term>)*
// <term>       ::= <factor> (("+" | "-") <factor>)*
// <factor>     ::= <unary> (("/" | "*") <unary>)*
// <unary>      ::= ("-" | "!") <unary> | <primary>
// <primary>    ::= NUMBER | STRING | "true" | "false" | "nil" | "("<expression>")"
// ---------------------------------------------------------------------------------------------------------------------

Ptr<ExprAst> Parser::Expression()
{
    return Equality();
}

Ptr<ExprAst> Parser::Assignment()
{
    auto expr = Or();

    if (Match(TokenType::Equal))
    {
        Token equals = Previous();
        Ptr<ExprAst> value = Assignment();

        if (auto var = std::dynamic_pointer_cast<VariableExpr>(expr); var)
        {
            Token name = var->name;
            return std::make_shared<AssignExpr>(name, value);
        }

        RaiseError(equals, "Invalid assignment target.");
    }

    return expr;
}

Ptr<ExprAst> Parser::Or()
{
    Ptr<ExprAst> expr = And();

    while (Match(TokenType::Or))
    {
        Token oper = Previous();
        Ptr<ExprAst> right = And();
        expr = std::make_shared<LogicalExpr>(expr, oper, right);
    }

    return expr;
}

Ptr<ExprAst> Parser::And()
{
    auto expr = Equality();

    while (Match(TokenType::And))
    {
        Token operatorTok = Previous();
        auto right = Equality();
        expr = std::make_shared<LogicalExpr>(expr, operatorTok, right);
    }

    return expr;
}

Ptr<ExprAst> Parser::Equality()
{
    using enum TokenType;

    auto expr = Comparison();

    while (Match(EqualEqual, NotEqual))
    {
        Token operatorTok = Previous();
        auto right = Comparison();

        expr = std::make_shared<BinaryExpr>(expr, operatorTok, right);
    }

    return expr;
}

Ptr<ExprAst> Parser::Comparison()
{
    using enum TokenType;

    auto expr = Addition();

    while (Match(More, MoreEqual, Less, LessEqual))
    {
        Token operatorTok = Previous();
        auto right = Addition();

        expr = std::make_shared<BinaryExpr>(expr, operatorTok, right);
    }

    return expr;
}

Ptr<ExprAst> Parser::Addition()
{
    using enum TokenType;

    auto expr = Multiplication();

    while (Match(Minus, Plus))
    {
        Token operatorTok = Previous();
        auto right = Multiplication();

        expr = std::make_shared<BinaryExpr>(expr, operatorTok, right);
    }

    return expr;
}

Ptr<ExprAst> Parser::Multiplication()
{
    using enum TokenType;

    auto expr = Unary();

    while (Match(Asterisk, Slash))
    {
        Token operatorTok = Previous();
        auto right = Unary();

        expr = std::make_shared<BinaryExpr>(expr, operatorTok, right);
    }

    return expr;
}

Ptr<ExprAst> Parser::Unary()
{
    using enum TokenType;

    if (Match(Not, Minus))
    {
        Token operatorTok = Previous();
        auto unary = Unary();

        return std::make_shared<UnaryExpr>(operatorTok, unary);
    }

    return Primary();
}

Ptr<ExprAst> Parser::Primary()
{
    using enum TokenType;
    using T = LiteralExpr;

    if (Match(False))
        return std::make_shared<T>(false);

    if (Match(True))
        return std::make_shared<T>(true);

    if (Match(Null))
        return std::make_shared<T>(nullptr);

    if (Match(Number, String))
        return std::make_shared<T>(Previous().Literal().value());

    if (Match(ParenLeft))
    {
        auto expr = Expression();
        Consume(ParenRight, "expected )");

        return std::make_shared<GroupingExpr>(expr);
    }

    RaiseError(Peek(), "expected expression");
    return nullptr;
}