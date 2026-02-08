#pragma once
#include "Ast.Forward.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

struct ExprVisitor
{
    virtual ~ExprVisitor() = default;

    virtual void Visit(const BinaryExpr& expr) = 0;
    virtual void Visit(const GroupingExpr& expr) = 0;
    virtual void Visit(const LiteralExpr& expr) = 0;
    virtual void Visit(const LogicalExpr& expr) = 0;
    virtual void Visit(const UnaryExpr& expr) = 0;
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core