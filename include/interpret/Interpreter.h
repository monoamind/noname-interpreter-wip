#pragma once
#include "Object.h"
#include "Visitor.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Interpreter : public ExprVisitor
{
    Object result_;

public:
    Interpreter() = default;
    ~Interpreter() override = default;

public:
    Object Result();

    void Visit(const BinaryExpr& expr) override;
    void Visit(const GroupingExpr& expr) override;
    void Visit(const LiteralExpr& expr) override;
    void Visit(const LogicalExpr& expr) override;
    void Visit(const UnaryExpr& expr) override;

    void Interpret(const ExprAst& root);

private:
    void Evaluate(const ExprAst& expr);
    void Execute(const StmtAst& stmt);
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core
