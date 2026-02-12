#pragma once
#include "Environment.h"
#include "Interpreter.h"
#include "Object.h"
#include "StatementAst.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

struct Return : std::runtime_error
{
    Object value;

    explicit Return(const Object& value)
        : std::runtime_error("")
        , value(value)
    {}
};

// ---------------------------------------------------------------------------------------------------------------------

class Function
{
    FunctionStmt declaration_;
    Ptr<Environment> closure_;

public:
    Function(FunctionStmt declaration, Ptr<Environment> closure)
        : declaration_(std::move(declaration))
        , closure_(std::move(closure))
    {}

    int Arity() const
    {
        return declaration_.params_.size();
    }

    Object Call(Interpreter* interpreter, const Vector<Object>& arguments)
    {
        auto env = std::make_shared<Environment>(closure_);

        for (size_t i = 0; i < Arity(); ++i)
        {
            env->Define(declaration_.params_[i].Lexeme(), arguments[i]);
        }

        try
        {
            interpreter->ExecuteBlock(declaration_.body, env);
        }
        catch (Return& returnValue)
        {
            return returnValue.value;
        }

        return nullptr;
    }

    std::string ToString() const
    {
        return "<fn " + declaration_.name.Lexeme() + ">";
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core