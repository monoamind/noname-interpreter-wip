#pragma once
#include "AstBase.h"
#include "Common.h"
#include "Object.h"
#include "Token.h"
#include "Visitor.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

struct AssignExpr : ExprAst
{
    Token name;
    Ptr<ExprAst> value = {};

public:
    explicit AssignExpr(const Token& name, Ptr<ExprAst> value)
        : name(name)
        , value(std::move(value))
    {}

    ~AssignExpr() override = default;

public:
    void Accept(ExprVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct BinaryExpr : ExprAst
{
    Ptr<ExprAst> left_ = {};
    Token operator_;
    Ptr<ExprAst> right_ = {};

public:
    explicit BinaryExpr(Ptr<ExprAst> left, Token op, Ptr<ExprAst> right)
        : left_(std::move(left))
        , operator_(std::move(op))
        , right_(std::move(right))
    {}

    ~BinaryExpr() override = default;

public:
    void Accept(ExprVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct CallExpr : ExprAst
{
    Ptr<ExprAst> callee = {};
    Token paren;
    PtrVector<ExprAst> args = {};

public:
    explicit CallExpr(Ptr<ExprAst> callee, const Token& paren, PtrVector<ExprAst> args)
        : callee(std::move(callee))
        , paren(paren)
        , args(std::move(args))
    {}

    ~CallExpr() override = default;

public:
    void Accept(ExprVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct GroupingExpr : ExprAst
{
    Ptr<ExprAst> expr_ = {};

public:
    explicit GroupingExpr(Ptr<ExprAst> expr)
        : expr_(std::move(expr))
    {}

    ~GroupingExpr() override = default;

public:
    void Accept(ExprVisitor& visitor) const override
    {
        return visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct LiteralExpr : ExprAst
{
    Object value_;

public:
    explicit LiteralExpr(const Object& value)
        : value_(std::move(value))
    {}

    ~LiteralExpr() override = default;

public:
    void Accept(ExprVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct LogicalExpr : ExprAst
{
    Ptr<ExprAst> left_;
    Token op_;
    Ptr<ExprAst> right_;

public:
    explicit LogicalExpr(Ptr<ExprAst> left, Token op, Ptr<ExprAst> right)
        : left_(std::move(left))
        , op_(std::move(op))
        , right_(std::move(right))
    {}

    ~LogicalExpr() override = default;

public:
    void Accept(ExprVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct UnaryExpr : ExprAst
{
    Token op_;
    Ptr<ExprAst> right_;

public:
    explicit UnaryExpr(Token op, Ptr<ExprAst> right)
        : op_(std::move(op))
        , right_(std::move(right))
    {}

    ~UnaryExpr() override = default;

public:
    void Accept(ExprVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

struct VariableExpr : ExprAst
{
    Token name;

public:
    explicit VariableExpr(const Token& name)
        : name(name)
    {}

    ~VariableExpr() override = default;

public:
    void Accept(ExprVisitor& visitor) const override
    {
        visitor.Visit(*this);
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core