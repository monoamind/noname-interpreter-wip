#include "Error.h"
#include "Parser.h"
#include "Runner.h"
#include "StatementAst.h"

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------

PtrVector<StmtAst> Parser::Parse()
{
    PtrVector<StmtAst> statements;

    while (!IsAtEnd())
        statements.emplace_back(Declaration());

    return statements;
}

// ---------------------------------------------------------------------------------------------------------------------

void Parser::RecoverFromError()
{
    using enum TokenType;
    Advance();

    while (!IsAtEnd())
    {
        if (Previous().IsOfType(Semicolon))
            return;

        if (Peek().IsOfType(Func, Var, For, If, While, Print, Return))
            return;

        Advance();
    }
}

ParseError Parser::RaiseError(const Token& token, const std::string& msg)
{
    Runner::Error(token, msg);
    return ParseError(msg);
}