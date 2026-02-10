#pragma once
#include <stdexcept>

#include "Token.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

struct ParseError : std::runtime_error
{
    explicit ParseError(const std::string& msg)
        : std::runtime_error(msg)
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

struct RuntimeError : std::runtime_error
{
    explicit RuntimeError(Token token, const std::string& msg)
        : std::runtime_error(msg)
        , token(token)
    {}

    Token token;
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core