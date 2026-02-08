#include "Error.h"
#include "ExprAst.h"
#include "Interpreter.h"

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------

void Interpreter::Interpret(const ExprAst& root)
{
    root.Accept(*this);
}

Object Interpreter::Result()
{
    return result_;
}

void Interpreter::Evaluate(const ExprAst& expr)
{
    expr.Accept(*this);
}

// ---------------------------------------------------------------------------------------------------------------------

void Interpreter::Visit(const BinaryExpr& expr)
{
    Evaluate(*expr.left_);
    auto left = result_;

    Evaluate(*expr.right_);
    auto right = result_;

    switch (expr.operator_.Type())
    {
        case TokenType::Plus:
            result_ = left + right;
            break;
        case TokenType::Minus:
            result_ = left - right;
            break;
        case TokenType::Slash:
            result_ = left / right;
            break;
        case TokenType::Asterisk:
            result_ = left * right;
            break;
        case TokenType::EqualEqual:
            result_ = (left == right);
            break;
        case TokenType::Less:
            result_ = (left < right);
            break;
        case TokenType::LessEqual:
            result_ = (left <= right);
            break;
        case TokenType::More:
            result_ = (left > right);
            break;
        case TokenType::MoreEqual:
            result_ = (left >= right);
            break;
        default:
            break;
    }
}

void Interpreter::Visit(const GroupingExpr& expr)
{
    Evaluate(*expr.expr_);
    auto x = result_;
}

void Interpreter::Visit(const LiteralExpr& expr)
{
    result_ = expr.value_;
}

void Interpreter::Visit(const LogicalExpr& expr)
{
    Evaluate(*expr.left_);
    auto left = result_;

    Evaluate(*expr.right_);
    auto right = result_;

    switch (expr.op_.Type())
    {
        case TokenType::And:
            result_ = left && right;
            break;
        case TokenType::Or:
            result_ = left || right;
            break;
        default:
            break;
    }
}

void Interpreter::Visit(const UnaryExpr& expr)
{
    Evaluate(*expr.right_);

    switch (expr.op_.Type())
    {
        case TokenType::Minus:
            result_ = -result_;
            break;
        case TokenType::Not:
            result_ = !result_;
            break;
        default:
            throw TypeError("Invalid type");
    }
}