#pragma once
#include "Ast.Forward.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

struct ExprVisitor
{
    virtual ~ExprVisitor() = default;

    virtual void Visit(const AssignExpr& expr) = 0;
    virtual void Visit(const BinaryExpr& expr) = 0;
    virtual void Visit(const CallExpr& expr) = 0;
    virtual void Visit(const GroupingExpr& expr) = 0;
    virtual void Visit(const LiteralExpr& expr) = 0;
    virtual void Visit(const LogicalExpr& expr) = 0;
    virtual void Visit(const UnaryExpr& expr) = 0;
    virtual void Visit(const VariableExpr& expr) = 0;
};

// ---------------------------------------------------------------------------------------------------------------------

struct StmtVisitor
{
    virtual ~StmtVisitor() = default;

    virtual void Visit(const BlockStmt& expr) = 0;
    virtual void Visit(const ExpressionStmt& expr) = 0;
    virtual void Visit(const ForStmt& expr) = 0;
    virtual void Visit(const FunctionStmt& expr) = 0;
    virtual void Visit(const IfStmt& expr) = 0;
    virtual void Visit(const PrintStmt& expr) = 0;
    virtual void Visit(const ReturnStmt& stmt) = 0;
    virtual void Visit(const VarStmt& expr) = 0;
    virtual void Visit(const WhileStmt& expr) = 0;
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core