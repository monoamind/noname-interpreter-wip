#pragma once
#include <stdexcept>

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class OperandError : public std::invalid_argument
{
    explicit OperandError(const std::string& msg)
        : std::invalid_argument(msg)
    {}
};

// ---------------------------------------------------------------------------------------------------------------------

struct TypeError : std::invalid_argument
{
    explicit TypeError(const std::string& msg)
        : std::invalid_argument(msg)
    {}
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Interpret
