#pragma once
#include "Object.h"
#include <deque>

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Environment
{
    using EnvPtr = std::shared_ptr<Environment>;

    EnvPtr enclosing_;
    std::deque<Object> values_;

public:
    Environment() = default;

    Environment(const EnvPtr& enclosing)
        : enclosing_(enclosing)
    {}

    void Define(Object value)
    {
        values_.emplace_back(value);
    }

    Object GetAt(int distance, int slot)
    {
        EnvPtr environment(this);

        for (int i = 0; i < distance; i++)
        {
            environment = environment->enclosing_;
        }

        return environment->values_[slot];
    }

    void AssignAt(int distance, int slot, const Object& value)
    {
        EnvPtr environment(this);

        for (int i = 0; i < distance; i++)
        {
            environment = environment->enclosing_;
        }

        environment->values_[slot] = value;
    }

    std::string ToString() const
    {
        std::string result;
        for (auto& value : values_)
        {
            result += value.ToString() + ' ';
        }

        if (enclosing_ != nullptr)
        {
            result += " -> " + enclosing_->ToString();
        }

        return result;
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core