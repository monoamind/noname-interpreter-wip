#include "Error.h"
#include "Parser.h"
#include "Runner.h"
#include "StatementAst.h"

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------

PtrVector<StmtAst> Parser::Parse()
{
    std::vector<Ptr<StmtAst>> statements;

    while (!IsAtEnd())
        statements.emplace_back(Declaration());

    return statements;
}

// ---------------------------------------------------------------------------------------------------------------------

bool Parser::IsAtEnd() const noexcept
{
    return Peek().IsEof();
}

const Token& Parser::Peek() const
{
    return tokens_[pos_];
}

const Token& Parser::Previous() const
{
    return tokens_[pos_ -  1];
}

const Token& Parser::Advance()
{
    if (!IsAtEnd())
        ++pos_;

    return Previous();
}

bool Parser::Check(TokenType type) const
{
    return !IsAtEnd() && Peek().IsOfType(type);
}

Token Parser::Consume(TokenType type, const std::string& errorMsg)
{
    if (!Check(type))
        throw RaiseError(Peek(), errorMsg);

    return tokens_[pos_++];
}

// ---------------------------------------------------------------------------------------------------------------------

void Parser::RecoverAfterError()
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