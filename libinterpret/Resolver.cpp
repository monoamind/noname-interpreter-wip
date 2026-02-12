#include "Resolver.h"
#include "AstBase.h"
#include "Runner.h"
#include "StatementAst.h"

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------

void Resolver::Resolve(const PtrVector<StmtAst>& statements)
{
    for (auto& stmt : statements)
    {
        Resolve(stmt);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void Resolver::Resolve(const Ptr<ExprAst>& stmt)
{
    stmt->Accept(*this);
}

void Resolver::Resolve(const Ptr<StmtAst>& stmt)
{
    stmt->Accept(*this);
}

void Resolver::ResolveFunction(const FunctionStmt* func)
{
    FunctionType enclosingFunc = currentFunc_;
    currentFunc_ = FunctionType::Function;

    BeginScope();

    for (auto& param : func->params_)
    {
        Declare(param);
        Define(param);
    }

    Resolve(func->body);
    EndScope();
    currentFunc_ = enclosingFunc;
}

void Resolver::ResolveLocal(const ExprAst* expr, const Token& name)
{
    int n = scopes_.size() - 1;
    auto lexeme = name.Lexeme();

    for (int i = n; i >= 0; --i)
    {
        if (auto& scope = scopes_[i]; scope.contains(lexeme))
        {
            interpreter_.Resolve(expr, n - i);
            return;
        }
    }
}

void Resolver::Declare(const Token& name)
{
    if (scopes_.empty())
        return;

    auto& scope = scopes_.back();

    if (scope.contains(name.Lexeme()))
    {
        Runner::Error(name, "Already a variable with this in this scope.");
    }

    scope[name.Lexeme()] = false;
}

void Resolver::Define(const Token& name)
{
    if (scopes_.empty())
        return;

    scopes_.back()[name.Lexeme()] = true;
}

void Resolver::BeginScope()
{
    scopes_.emplace_back(Scope{});
}

void Resolver::EndScope()
{
    scopes_.pop_back();
}

// ---------------------------------------------------------------------------------------------------------------------

void Resolver::Visit(const BlockStmt* stmt)
{
    BeginScope();
    Resolve(stmt->statements);
    EndScope();
}

void Resolver::Visit(const ExpressionStmt* stmt)
{
    Resolve(stmt->expr);
}

void Resolver::Visit(const FunctionStmt* stmt)
{
    Declare(stmt->name);
    Define(stmt->name);
    ResolveFunction(stmt);
}

void Resolver::Visit(const IfStmt* stmt)
{
    Resolve(stmt->condition);
    Resolve(stmt->thenBranch);

    if (stmt->elseBranch)
        Resolve(stmt->elseBranch);
}

void Resolver::Visit(const PrintStmt* stmt)
{
    Resolve(stmt->expression);
}

void Resolver::Visit(const ReturnStmt* stmt)
{
    if (currentFunc_ == FunctionType::None)
        Runner::Error(stmt->keyword, "Can't return from top-level code.");

    if (stmt->value)
        Resolve(stmt->value);
}

void Resolver::Visit(const VarStmt* stmt)
{
    Declare(stmt->name);

    if (stmt->initializer)
        Resolve(stmt->initializer);

    Define(stmt->name);
}

void Resolver::Visit(const WhileStmt* stmt)
{
    Resolve(stmt->condition);
    Resolve(stmt->body);
}

// ---------------------------------------------------------------------------------------------------------------------

void Resolver::Visit(const AssignExpr* expr)
{
    Resolve(expr->value);
    ResolveLocal(expr, expr->name);
}

void Resolver::Visit(const BinaryExpr* expr)
{
    Resolve(expr->left);
    Resolve(expr->right);
}

void Resolver::Visit(const CallExpr* expr)
{
    Resolve(expr->callee);

    for (auto& arg : expr->args)
        Resolve(arg);
}

void Resolver::Visit(const GroupingExpr* expr)
{
    Resolve(expr->expr);
}

void Resolver::Visit(const LiteralExpr* expr)
{}

void Resolver::Visit(const LogicalExpr* expr)
{
    Resolve(expr->left);
    Resolve(expr->right);
}

void Resolver::Visit(const UnaryExpr* expr)
{
    Resolve(expr->right);
}

void Resolver::Visit(const VariableExpr* expr)
{
    auto strName = expr->name.Lexeme();

    if (!scopes_.empty()
        && scopes_.back().contains(strName)
        && scopes_.back().at(strName) == false)
    {
        Runner::Error(expr->name, "Can't read local variable in its own initializer.");
    }

    ResolveLocal(expr, expr->name);
}