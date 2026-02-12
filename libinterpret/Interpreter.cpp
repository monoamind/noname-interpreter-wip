#include "Error.h"
#include "ExpressionAst.h"
#include "Interpreter.h"
#include "Function.h"
#include "Runner.h"
#include "StatementAst.h"

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------

void Interpreter::Interpret(const PtrVector<StmtAst>& statements)
{
    try
    {
        for (auto& stmt : statements)
        {
            Execute(stmt.get());
        }
    }
    catch (TypeError& err)
    {
        std::cerr << err.what() << std::endl;
    }
    catch (RuntimeError& err)
    {
        Runner::RuntimeError(err);
    }
}

void Interpreter::Resolve(const ExprAst* expr, int depth)
{
    locals_[expr] = depth;
}

void Interpreter::ExecuteBlock(const PtrVector<StmtAst>& stmts, Ptr<Environment> env)
{
    auto prev = env_;
    auto finallyFunc = [this, &prev](void*)
    {
        env_ = prev;
    };

    std::unique_ptr<void, decltype(finallyFunc)> scope_guard((void*)1, finallyFunc);

    env_ = env;

    for (auto& stmt : stmts)
    {
        Execute(stmt.get());
    }
}

Object Interpreter::Result()
{
    return result_;
}

void Interpreter::Reset()
{
    globals_ = std::make_shared<Environment>();
    env_ = globals_;
    locals_.clear();
}

Object Interpreter::Evaluate(const ExprAst* expr)
{
    expr->Accept(*this);
    return result_;
}

void Interpreter::Execute(const StmtAst* stmt)
{
    stmt->Accept(*this);
}

// ---------------------------------------------------------------------------------------------------------------------

void Interpreter::Visit(const BlockStmt* block)
{
    ExecuteBlock(block->statements, std::make_shared<Environment>(env_));
}

void Interpreter::Visit(const ExpressionStmt* stmt)
{
    Evaluate(stmt->expr.get());
}

void Interpreter::Visit(const FunctionStmt* stmt)
{
    Object func = std::make_shared<Function>(*stmt, env_);
    env_->Define(stmt->name.Lexeme(), func);
}

void Interpreter::Visit(const IfStmt* stmt)
{
    if (IsTruthy(Evaluate(stmt->condition.get())))
    {
        Execute(stmt->thenBranch.get());
    }
    else if (stmt->elseBranch)
    {
        Execute(stmt->elseBranch.get());
    }
}

void Interpreter::Visit(const PrintStmt* stmt)
{
    Object value = Evaluate(stmt->expression.get());
    std::cout << Stringify(value) << std::endl;
}

void Interpreter::Visit(const ReturnStmt* stmt)
{
    if (stmt->value)
    {
        throw Return(Evaluate(stmt->value.get()));
    }

    throw Return(nullptr);
}

void Interpreter::Visit(const VarStmt* stmt)
{
    Object value;

    if (stmt->initializer)
        value = Evaluate(stmt->initializer.get());

    env_->Define(stmt->name.Lexeme(), value);
}

void Interpreter::Visit(const WhileStmt* stmt)
{
    while (IsTruthy(Evaluate(stmt->condition.get())))
    {
        Execute(stmt->body.get());
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void Interpreter::Visit(const AssignExpr* expr)
{
    Object value = Evaluate(expr->value.get());

    if (locals_.contains(expr))
    {
        int distance = locals_.at(expr);
        env_->AssignAt(distance, expr->name, value);
        result_ = value;
        return;
    }

    globals_->Assign(expr->name, value);
    result_ = value;
}

void Interpreter::Visit(const BinaryExpr* expr)
{
    Object left = Evaluate(expr->left.get());
    Object right = Evaluate(expr->right.get());

    switch (expr->operatorToken.Type())
    {
        case TokenType::EqualEqual:
            result_ = IsEqual(left, right);
            break;
        case TokenType::NotEqual:
            result_ = !IsEqual(left, right);
            break;
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

void Interpreter::Visit(const CallExpr* expr)
{
    Object callee = Evaluate(expr->callee.get());
    Vector<Object> arguments = {};
    
    for (auto& a : expr->args)
        arguments.push_back(Evaluate(a.get()));
    
    auto func = std::dynamic_pointer_cast<Function>(callee.ToFunction());
    if (!func)
    {
        throw RuntimeError(expr->paren, "Can only call functions.");
    }

    auto argsCount = arguments.size();
    auto funcArity = func->Arity();

    if (argsCount != funcArity)
    {
        constexpr static auto fmtStr = "Expected {} arguments, but got {}.";
        auto fmt = std::format(fmtStr, funcArity, argsCount);

        throw RuntimeError(expr->paren, fmt);
    }

    result_ = func->Call(this, arguments);
}

void Interpreter::Visit(const GroupingExpr* expr)
{
    Evaluate(expr->expr.get());
}

void Interpreter::Visit(const LiteralExpr* expr)
{
    result_ = expr->value;
}

void Interpreter::Visit(const LogicalExpr* expr)
{
    auto left = Evaluate(expr->left.get());

    if (expr->op.IsOfType(TokenType::Or))
    {
        if (IsTruthy(left))
            result_ = left;
    }
    else if (expr->op.IsOfType(TokenType::And))
    {
        if (!IsTruthy(left))
            result_ = left;
    }

    Evaluate(expr->right.get());
}

void Interpreter::Visit(const UnaryExpr* expr)
{
    Object right = Evaluate(expr->right.get());

    switch (expr->op.Type())
    {
        case TokenType::Minus:
            CheckNumberOperand(expr->op, right);
            result_ = -right.ToDouble();
            break;
        case TokenType::Not:
            result_ = !IsTruthy(right);
            break;
        default:
            break;
    }
}

void Interpreter::Visit(const VariableExpr* expr)
{
    auto var = LookUpVar(expr->name, expr);
    result_ = var;
}

// ---------------------------------------------------------------------------------------------------------------------

Object Interpreter::LookUpVar(const Token& name, const ExprAst* expr)
{
    if (locals_.contains(expr))
    {
        int distance = locals_.at(expr);
        return env_->GetAt(distance, name.Lexeme());
    }

    return globals_->Get(name);
}

void Interpreter::CheckNumberOperand(const Token& op, const Object& operand)
{
    if (operand.IsDouble())
        return;

    throw new RuntimeError(op, "Operand must be a number.");
}

void Interpreter::CheckNumberOperands(const Token& op, const Object& l, const Object& r)
{
    if (l.IsDouble() && r.IsDouble())
        return;

    throw new RuntimeError(op, "Operands must be numbers.");
}

bool Interpreter::IsTruthy(const Object& obj)
{
    if (obj.IsNull())
        return false;

    if (obj.IsBool())
        return obj.ToBool();

    return true;
}

bool Interpreter::IsEqual(const Object& l, const Object& r)
{
    if (l.IsNull() && r.IsNull())
        return true;

    if (l.IsNull())
        return false;

    return l == r;
}

std::string Interpreter::Stringify(const Object& object)
{
    if (object.IsNull())
        return "Null";

    return object.ToString();
}