#pragma once
#include "Interpreter.h"
#include "Token.h"
#include "Visitor.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Resolver : public ExprVisitor, public StmtVisitor
{
    enum class FunctionType
    {
        None,
        Function
    };

    using Scope = std::map<std::string, bool>;

private:
    Interpreter& interpreter_;
    std::deque<Scope> scopes_ = {};
    FunctionType currentFunc_ = FunctionType::None;

public:
    explicit Resolver(Interpreter& interpreter)
        : interpreter_(interpreter)
    {}

public:
    void Visit(const AssignExpr* expr) override;
    void Visit(const BinaryExpr* expr) override;
    void Visit(const CallExpr* expr) override;
    void Visit(const GroupingExpr* expr) override;
    void Visit(const LiteralExpr* expr) override;
    void Visit(const LogicalExpr* expr) override;
    void Visit(const UnaryExpr* expr) override;
    void Visit(const VariableExpr* expr) override;

    void Visit(const BlockStmt* stmt) override;
    void Visit(const ExpressionStmt* stmt) override;
    void Visit(const FunctionStmt* stmt) override;
    void Visit(const IfStmt* stmt) override;
    void Visit(const PrintStmt* stmt) override;
    void Visit(const ReturnStmt* stmt) override;
    void Visit(const VarStmt* stmt) override;
    void Visit(const WhileStmt* expr) override;

    void Resolve(const PtrVector<StmtAst>& statements);

private:
    void Resolve(const Ptr<StmtAst>& stmt);
    void Resolve(const Ptr<ExprAst>& expr);

    void ResolveFunction(const FunctionStmt* func);
    void ResolveLocal(const ExprAst* expr, const Token& type);

    void BeginScope();
    void EndScope();

    void Declare(const Token& name);
    void Define(const Token& name);
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core