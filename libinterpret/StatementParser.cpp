#include "Parser.h"
#include "StatementAst.h"

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
// <program>    ::= <statement>* EOF
// <statement>  ::= <expr_stmt> | <print_stmt>
// <expr_stmt>  ::= expression ";"
// <print_stmt> ::= "print" <expression> ";"
// ---------------------------------------------------------------------------------------------------------------------

Ptr<StmtAst> Parser::Statement()
{
    using enum TokenType;

    if (Match(For))
        return ForStatement();

    if (Match(If))
        return IfStatement();

    if (Match(Print))
        return PrintStatement();

    if (Match(Return))
        return ReturnStatement();

    if (Match(While))
        return WhileStatement();

    if (Match(BraceLeft))
        return std::make_shared<BlockStmt>(Block());

    return ExpressionStatement();
}

Ptr<StmtAst> Parser::Declaration()
{
    try
    {
        if (Match(TokenType::Func))
            return nullptr; // Function("function");

        if (Match(TokenType::Var))
            return VarDeclaration();

        return Statement();
    }
    catch (ParseError& error)
    {
        RecoverFromError();
        return nullptr;
    }
}

Ptr<StmtAst> Parser::VarDeclaration()
{
    Token name = Consume(TokenType::Identifier, "Expect variable name.");
    Ptr<ExprAst> initializer = nullptr;

    if (Match(TokenType::Equal))
        initializer = Expression();

    Consume(TokenType::Semicolon, "Expect ';' after variable declaration.");
    return std::make_shared<VarStmt>(name, initializer);
}

PtrVector<StmtAst> Parser::Block()
{
    PtrVector<StmtAst> statements;

    while (!Check(TokenType::BraceRight) && !IsAtEnd())
    {
        statements.emplace_back(Declaration());
    }

    Consume(TokenType::BraceRight, "Expect '}' after block.");
    return statements;
}

Ptr<StmtAst> Parser::ExpressionStatement()
{
    auto expr = Expression();
    Consume(TokenType::Semicolon, "Expected ';' after value.");

    return std::make_shared<ExpressionStmt>(expr);
}

Ptr<StmtAst> Parser::ForStatement()
{
    using enum TokenType;

    Consume(ParenLeft, "Expect '(' after 'for'.");

    Ptr<StmtAst> initializer;

    if (Match(Semicolon))
    {
        initializer = nullptr;
    }
    else if (Match(Var))
    {
        initializer = VarDeclaration();
    }
    else
    {
        initializer = ExpressionStatement();
    }

    Ptr<ExprAst> condition = nullptr;

    if (!Check(Semicolon))
        condition = Expression();

    Consume(Semicolon, "Expect ';' after loop condition.");

    Ptr<ExprAst> increment = nullptr;

    if (!Check(ParenRight))
        increment = Expression();

    Consume(ParenRight, "Expect ')' after for clauses.");

    Ptr<StmtAst> body = Statement();

    if (increment != nullptr)
    {
        body = std::make_shared<BlockStmt>(PtrVector<StmtAst>
        {
            body,
            std::make_shared<ExpressionStmt>(increment)
        });
    }

    if (!condition)
        condition = std::make_shared<LiteralExpr>(true);

    body = std::make_shared<WhileStmt>(condition, body);

    if (initializer)
        body = std::make_shared<BlockStmt>(PtrVector<StmtAst>{initializer, body});

    return body;
}

Ptr<StmtAst> Parser::IfStatement()
{
    Consume(TokenType::ParenLeft, "Expect '(' after 'if'.");
    auto condition = Expression();
    Consume(TokenType::ParenRight, "Expect ')' after if condition.");

    Ptr<StmtAst> thenBranch = Statement();
    Ptr<StmtAst> elseBranch = nullptr;

    if (Match(TokenType::Else))
        elseBranch = Statement();

    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

Ptr<StmtAst> Parser::PrintStatement()
{
    auto value = Expression();
    Consume(TokenType::Semicolon, "Expect ';' after value.");

    return std::make_shared<PrintStmt>(value);
}

Ptr<StmtAst> Parser::ReturnStatement()
{
    Token keyword = Previous();
    Ptr<ExprAst> value = nullptr;

    if (!Check(TokenType::Semicolon))
    {
        value = Expression();
    }

    Consume(TokenType::Semicolon, "Expect ';' after return value.");
    return std::make_shared<ReturnStmt>(keyword, value);
}

Ptr<StmtAst> Parser::WhileStatement()
{
    Consume(TokenType::ParenLeft, "Expect '(' after 'while'.");
    Ptr<ExprAst> condition = Expression();

    Consume(TokenType::ParenRight, "Expect ')' after condition.");
    Ptr<StmtAst> body = Statement();

    return std::make_shared<WhileStmt>(condition, body);
}
