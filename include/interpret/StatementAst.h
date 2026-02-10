#pragma once
#include "AstBase.h"
#include "Common.h"
#include "Token.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

struct BlockStmt : StmtAst
{
    PtrVector<StmtAst> statements_ = {};

public:
    explicit BlockStmt(PtrVector<StmtAst> statements)
        : statements_(std::move(statements))
    {}

    ~BlockStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct ExpressionStmt : StmtAst
{
    Ptr<ExprAst> expr_ = {};

public:
    explicit ExpressionStmt(Ptr<ExprAst> expr)
        : expr_(std::move(expr))
    {}

    ~ExpressionStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct FunctionStmt : StmtAst
{
    Token name_;
    Vector<Token> params_ = {};
    PtrVector<StmtAst> body_ = {};

public:
    explicit FunctionStmt(const Token& name, Vector<Token> params, PtrVector<StmtAst> body)
        : name_(name)
        , params_(std::move(params))
        , body_(std::move(body))
    {}

    ~FunctionStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct IfStmt : StmtAst
{
    Ptr<ExprAst> condition_ = {};
    Ptr<StmtAst> thenBranch_ = {};
    Ptr<StmtAst> elseBranch_ = {};

public:
    explicit IfStmt(Ptr<ExprAst> condition, Ptr<StmtAst> thenBranch, Ptr<StmtAst> elseBranch)
        : condition_(std::move(condition))
        , thenBranch_(std::move(thenBranch))
        , elseBranch_(std::move(elseBranch))
    {}

    ~IfStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct PrintStmt : StmtAst
{
    Ptr<ExprAst> expression_ = {};

public:
    explicit PrintStmt(Ptr<ExprAst> expression)
        : expression_(std::move(expression))
    {}

    ~PrintStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct ReturnStmt : StmtAst
{
    Token keyword_;
    Ptr<ExprAst> value_;

public:
    explicit ReturnStmt(const Token& token, Ptr<ExprAst> expr)
        : keyword_(token)
        , value_(std::move(expr))
    {}

    ~ReturnStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct VarStmt : StmtAst
{
    Token name_;
    Ptr<ExprAst> initializer_ = {};

public:
    explicit VarStmt(Token name, Ptr<ExprAst> init)
        : name_(std::move(name))
        , initializer_(std::move(init))
    {}

    ~VarStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct WhileStmt : StmtAst
{
    Ptr<ExprAst> condition = {};
    Ptr<StmtAst> body = {};

public:
    explicit WhileStmt(Ptr<ExprAst> condition, Ptr<StmtAst> body)
        : condition(std::move(condition))
        , body(std::move(body))
    {}

    ~WhileStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core