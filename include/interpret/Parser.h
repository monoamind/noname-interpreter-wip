#pragma once
#include "Common.h"
#include "Error.h"
#include "ExpressionAst.h"

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
    Ptr<FunctionStmt> Function(const std::string& kind);

    Ptr<ExprAst> Expression();
    Ptr<ExprAst> Assignment();
    Ptr<ExprAst> Or();
    Ptr<ExprAst> And();
    Ptr<ExprAst> Equality();
    Ptr<ExprAst> Comparison();
    Ptr<ExprAst> Addition();
    Ptr<ExprAst> Multiplication();
    Ptr<ExprAst> Unary();
    Ptr<ExprAst> FinishCall(Ptr<ExprAst> callee);
    Ptr<ExprAst> Call();
    Ptr<ExprAst> Primary();

    Ptr<StmtAst> Statement();
    Ptr<StmtAst> Declaration();
    Ptr<StmtAst> VarDeclaration();
    Ptr<StmtAst> ExpressionStatement();
    Ptr<StmtAst> ForStatement();
    Ptr<StmtAst> IfStatement();
    Ptr<StmtAst> PrintStatement();
    Ptr<StmtAst> ReturnStatement();
    Ptr<StmtAst> WhileStatement();

private:
    bool IsAtEnd() const noexcept;

    const Token& Peek() const;
    const Token& Previous() const;
    const Token& Advance();

    Token Consume(TokenType type, const std::string& errorMsg);

    bool Check(TokenType type) const;
    bool Match(auto... types);

private:
    void RecoverAfterError();
    static ParseError RaiseError(const Token& token, const std::string& msg);
};

// ---------------------------------------------------------------------------------------------------------------------

bool Parser::Match(auto... types)
{
    if (bool result = (Peek().IsOfType(types) || ...); result)
    {
        ++pos_;
        return true;
    }

    return false;
}

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core