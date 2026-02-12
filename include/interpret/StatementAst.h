#pragma once
#include "AstBase.h"
#include "Common.h"
#include "Token.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

struct BlockStmt : StmtAst
{
    PtrVector<StmtAst> statements = {};

public:
    explicit BlockStmt(PtrVector<StmtAst> statements)
        : statements(std::move(statements))
    {}

    ~BlockStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct ExpressionStmt : StmtAst
{
    Ptr<ExprAst> expr = {};

public:
    explicit ExpressionStmt(Ptr<ExprAst> expr)
        : expr(std::move(expr))
    {}

    ~ExpressionStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct FunctionStmt : StmtAst
{
    Token name;
    Vector<Token> params_ = {};
    PtrVector<StmtAst> body = {};

public:
    explicit FunctionStmt(const Token& name, Vector<Token> params, PtrVector<StmtAst> body)
        : name(name)
        , params_(std::move(params))
        , body(std::move(body))
    {}

    ~FunctionStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct IfStmt : StmtAst
{
    Ptr<ExprAst> condition = {};
    Ptr<StmtAst> thenBranch = {};
    Ptr<StmtAst> elseBranch = {};

public:
    explicit IfStmt(Ptr<ExprAst> condition, Ptr<StmtAst> thenBranch, Ptr<StmtAst> elseBranch)
        : condition(std::move(condition))
        , thenBranch(std::move(thenBranch))
        , elseBranch(std::move(elseBranch))
    {}

    ~IfStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct PrintStmt : StmtAst
{
    Ptr<ExprAst> expression = {};

public:
    explicit PrintStmt(Ptr<ExprAst> expression)
        : expression(std::move(expression))
    {}

    ~PrintStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct ReturnStmt : StmtAst
{
    Token keyword;
    Ptr<ExprAst> value;

public:
    explicit ReturnStmt(const Token& token, Ptr<ExprAst> expr)
        : keyword(token)
        , value(std::move(expr))
    {}

    ~ReturnStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct VarStmt : StmtAst
{
    Token name;
    Ptr<ExprAst> initializer = {};

public:
    explicit VarStmt(Token name, Ptr<ExprAst> init)
        : name(std::move(name))
        , initializer(std::move(init))
    {}

    ~VarStmt() override = default;

public:
    void Accept(StmtVisitor& visitor) const override
    {
        visitor.Visit(this);
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
        visitor.Visit(this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core