#pragma once
#include "Common.h"
#include "Object.h"
#include "Token.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Environment
{
    using EnvPtr = std::shared_ptr<Environment>;

    EnvPtr enclosing_ = {};
    std::map<std::string, Object> values_ = {};

public:
    Environment() = default;

    explicit Environment(const EnvPtr& enclosing)
        : enclosing_(enclosing)
    {}

public:
    Environment* Ancestor(int distance);

    Object Get(const Token& name) const;
    Object GetAt(int distance, const std::string& name);

    void Assign(const Token& name, const Object& value);
    void AssignAt(int distance, const Token& name, const Object& value);
    void Define(const std::string& name, const Object& value);

    std::string ToString() const;
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core