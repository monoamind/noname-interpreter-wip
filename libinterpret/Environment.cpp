#include "Environment.h"
#include "Error.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

Object Environment::Get(const Token& name) const
{
    if (values_.contains(name.Lexeme()))
        return values_.at(name.Lexeme());

    if (enclosing_)
        return enclosing_->Get(name);

    throw RuntimeError(name, "Undefined variable '" + name.Lexeme() + "'.");
}

Object Environment::GetAt(int distance, const std::string& name)
{
    if (auto env = Ancestor(distance); env)
    {
        return env->values_.at(name);
    }

    throw std::invalid_argument("Wrong distance.");
}

void Environment::Assign(const Token& name, const Object& value)
{
    if (values_.contains(name.Lexeme()))
    {
        values_[name.Lexeme()] = value;
        return;
    }

    if (enclosing_)
    {
        enclosing_->Assign(name, value);
        return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.Lexeme() + "'.");
}

void Environment::AssignAt(int distance, const Token& name, const Object& value)
{
    if (auto env = Ancestor(distance); env)
    {
        env->values_[name.Lexeme()] = value;
    }
    else
    {
        throw std::invalid_argument("Wrong distance.");
    }
}

void Environment::Define(const std::string& name, const Object& value)
{
    values_[name] = value;
}

Environment* Environment::Ancestor(int distance)
{
    Environment* environment = this;

    for (int i = 0; i < distance; i++)
    {
        // std::cout << environment->ToString();
        environment = environment->enclosing_.get();
    }

    return environment;
}

std::string Environment::ToString() const
{
    std::string result;

    for (auto& [name, value] : values_)
    {
        result += name + ": " + value.ToString() + '\n';
    }

    if (enclosing_ != nullptr)
    {
        result += " -> " + enclosing_->ToString();
    }

    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core