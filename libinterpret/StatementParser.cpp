#include "Parser.h"
#include "Runner.h"
#include "StatementAst.h"

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
// <program>     ::= <declaration>* EOF;
// <declaration> ::= <func_decl> | <var_decl> | <statement>
// <func_decl>   ::= "func" <function>
// <function>    ::= ID "(" <parameters>? ")" block
// <parameters>  ::= ID ("," ID)*
// <block>       ::= "{" <declaration> "}"
// <var_decl>    ::= "var" ID ("=" <expression>)? ";"
// <statement>   ::= <expr_stmt> | <block> | <while_stmt> | <if_stmt> | <for_stmt> | <return_stmt> | <print_stmt>
// <expr_stmt>   ::= <expression> ";"
// <while_stmt>  ::= "while" "(" <expression> ")" <statement>
// <if_stmt>     ::= "if" "(" <expression> ")" <statement> ("else" <statement>)?
// <for_stmt>    ::= "for" "(" (<var_decl> | <expr_stmt> | ";") <expression>? ";" <expression>? ")" <statement>
// <return_stmt> ::= "return" <expression>? ";"
// <print_stmt>  ::= "print" <expression> ";"
// ---------------------------------------------------------------------------------------------------------------------

PtrVector<StmtAst> Parser::Block()
{
    PtrVector<StmtAst> statements;

    while (!Check(TokenType::BraceRight) && !IsAtEnd())
    {
        statements.emplace_back(Declaration());
    }

    Consume(TokenType::BraceRight, "Expected '}' after block.");
    return statements;
}

Ptr<FunctionStmt> Parser::Function(const std::string& kind)
{
    using enum TokenType;

    Token name = Consume(Identifier, "Expected " + kind + " name.");
    Consume(ParenLeft, "Expected '(' after " + kind + " name.");

    Vector<Token> parameters = {};

    if (!Check(ParenRight))
    {
        do
        {
            if (parameters.size() >= 255)
                RaiseError(Peek(), "Can't have more than 255 parameters.");

            parameters.emplace_back(Consume(Identifier, "Expect parameter name."));
        }
        while (Match(Comma));
    }

    Consume(ParenRight, "Expected ')' after parameters.");
    Consume(BraceLeft, "Expected '{' before " + kind + " body.");

    auto body = Block();
    return std::make_shared<FunctionStmt>(name, parameters, body);
}

Ptr<StmtAst> Parser::Declaration()
{
    try
    {
        if (Match(TokenType::Func))
            return Function("function");

        if (Match(TokenType::Var))
            return VarDeclaration();

        return Statement();
    }
    catch (ParseError& error)
    {
        Runner::Error(line_, error.what());
        RecoverAfterError();
        return nullptr;
    }
}

Ptr<StmtAst> Parser::VarDeclaration()
{
    Token name = Consume(TokenType::Identifier, "Expected variable name.");
    Ptr<ExprAst> initializer = nullptr;

    if (Match(TokenType::Equal))
        initializer = Expression();

    Consume(TokenType::Semicolon, "Expected ';' after variable declaration.");
    return std::make_shared<VarStmt>(name, initializer);
}

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

Ptr<StmtAst> Parser::ExpressionStatement()
{
    auto expr = Expression();
    Consume(TokenType::Semicolon, "Expected ';' after value.");

    return std::make_shared<ExpressionStmt>(expr);
}

Ptr<StmtAst> Parser::ForStatement()
{
    using enum TokenType;

    Consume(ParenLeft, "Expected '(' after 'for'.");

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

    Consume(Semicolon, "Expected ';' after loop condition.");

    Ptr<ExprAst> increment = nullptr;

    if (!Check(ParenRight))
        increment = Expression();

    Consume(ParenRight, "Expected ')' after for clauses.");

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
    Consume(TokenType::ParenLeft, "Expected '(' after 'if'.");
    auto condition = Expression();
    Consume(TokenType::ParenRight, "Expected ')' after if condition.");

    Ptr<StmtAst> thenBranch = Statement();
    Ptr<StmtAst> elseBranch = nullptr;

    if (Match(TokenType::Else))
        elseBranch = Statement();

    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

Ptr<StmtAst> Parser::PrintStatement()
{
    auto value = Expression();
    Consume(TokenType::Semicolon, "Expected ';' after value.");

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

    Consume(TokenType::Semicolon, "Expected ';' after return value.");
    return std::make_shared<ReturnStmt>(keyword, value);
}

Ptr<StmtAst> Parser::WhileStatement()
{
    Consume(TokenType::ParenLeft, "Expected '(' after 'while'.");
    Ptr<ExprAst> condition = Expression();

    Consume(TokenType::ParenRight, "Expected ')' after condition.");
    Ptr<StmtAst> body = Statement();

    return std::make_shared<WhileStmt>(condition, body);
}