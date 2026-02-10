#pragma once
#include "Common.h"
#include "Error.h"
#include "ExpressionAst.h"
#include "Scanner.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Parser
{
    std::vector<Token> tokens_ = {};
    size_t pos_ = 0;
    size_t line_ = 0;

public:
    explicit Parser(const std::vector<Token>& tokens)
        : tokens_(tokens)
    {}

public:
    PtrVector<StmtAst> Parse();

private:
    PtrVector<StmtAst> Block();

    Ptr<ExprAst> Expression();
    Ptr<ExprAst> Assignment();
    Ptr<ExprAst> Or();
    Ptr<ExprAst> And();
    Ptr<ExprAst> Equality();
    Ptr<ExprAst> Comparison();
    Ptr<ExprAst> Addition();
    Ptr<ExprAst> Multiplication();
    Ptr<ExprAst> Unary();
    Ptr<ExprAst> Primary();

    Ptr<StmtAst> Statement();
    Ptr<StmtAst> Declaration();
    Ptr<StmtAst> VarDeclaration();

    Ptr<StmtAst> ExpressionStatement();
    Ptr<StmtAst> ForStatement();
    // Ptr<StmtAst> FunctionStatement();
    Ptr<StmtAst> IfStatement();
    Ptr<StmtAst> PrintStatement();
    Ptr<StmtAst> ReturnStatement();
    Ptr<StmtAst> VarStatement();
    Ptr<StmtAst> WhileStatement();

    void RecoverFromError();

private:
    bool IsAtEnd() const noexcept
    {
        return Peek().IsEof();
    }

    const Token& Peek() const
    {
        return tokens_[pos_];
    }

    const Token& Previous() const
    {
        return tokens_[pos_ -  1];
    }

    const Token& Advance()
    {
        if (!IsAtEnd())
            ++pos_;

        return Previous();
    }

    void MoveBack()
    {
        if (pos_ > 0)
            pos_--;
    }

    bool Check(TokenType type) const
    {
        return !IsAtEnd() && Peek().IsOfType(type);
    }

    Token Consume(TokenType type, const std::string& errorMsg)
    {
        if (!Check(type))
            throw RaiseError(Peek(), errorMsg);

        return tokens_[pos_++];
    }

    bool Match(auto... types)
    {
        if (bool result = (Peek().IsOfType(types) || ...); result)
        {
            ++pos_;
            return true;
        }

        return false;
    }

private:
    ParseError RaiseError(const Token& token, const std::string& msg);
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core