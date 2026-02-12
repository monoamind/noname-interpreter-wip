#pragma once
#include "Common.h"
#include "Environment.h"
#include "Object.h"
#include "Token.h"
#include "Visitor.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Interpreter : public ExprVisitor, public StmtVisitor
{
    std::shared_ptr<Environment> globals_;
    std::shared_ptr<Environment> env_;
    std::map<const ExprAst*, int> locals_ = {};
    Object result_;

public:
    Interpreter()
        : globals_(std::make_shared<Environment>())
        , env_(globals_)
    {}

    ~Interpreter() override = default;

public:
    Object Result();
    void Reset();

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

    void Interpret(const PtrVector<StmtAst>& statements);
    void Resolve(const ExprAst* expr, int depth);
    void ExecuteBlock(const PtrVector<StmtAst>& stmts, Ptr<Environment> env);

private:
    Object Evaluate(const ExprAst* expr);
    void Execute(const StmtAst* stmt);

    Object LookUpVar(const Token& name, const ExprAst* expr);

    static void CheckNumberOperand(const Token& op, const Object& operand);
    static void CheckNumberOperands(const Token& op, const Object& l, const Object& r);

    static bool IsTruthy(const Object& obj);
    static bool IsEqual(const Object& l, const Object& r);

    static std::string Stringify(const Object& object);
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core
