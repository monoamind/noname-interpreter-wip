#pragma once
#include "Common.h"
#include "Environment.h"
#include "Object.h"
#include "Visitor.h"

#include <map>

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Interpreter : public ExprVisitor, StmtVisitor
{
    Object result_;

public:
    Interpreter() = default;
    ~Interpreter() override = default;

public:
    Object Result();

    void Visit(const AssignExpr& expr) override;
    void Visit(const BinaryExpr& expr) override;
    void Visit(const CallExpr& expr) override;
    void Visit(const GroupingExpr& expr) override;
    void Visit(const LiteralExpr& expr) override;
    void Visit(const LogicalExpr& expr) override;
    void Visit(const UnaryExpr& expr) override;
    void Visit(const VariableExpr& expr) override;

    void Visit(const BlockStmt& stmt) override;
    void Visit(const ExpressionStmt& stmt) override;
    void Visit(const ForStmt& stmt) override;
    void Visit(const FunctionStmt& stmt) override;
    void Visit(const IfStmt& stmt) override;
    void Visit(const PrintStmt& stmt) override;
    void Visit(const ReturnStmt& stmt) override;
    void Visit(const VarStmt& stmt) override;
    void Visit(const WhileStmt& expr) override;

    void Interpret(const PtrVector<StmtAst>& statements);

private:
    void Evaluate(const ExprAst& expr);
    void Execute(const StmtAst& stmt);

private:
    std::map<std::string, Object> globals_;
    std::shared_ptr<Environment> env_;

};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core
