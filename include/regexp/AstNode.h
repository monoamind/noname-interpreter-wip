#pragma once
#include "NFA.h"
#include <format>

namespace Regex
{
// ---------------------------------------------------------------------------------------------------------------------

class AstNode
{
public:
    using AstNodePtr = std::shared_ptr<AstNode>;

public:
    virtual ~AstNode() = default;

public:
    virtual NFA ToNFA() const = 0;
    virtual std::string ToString() const = 0;
    virtual size_t Precedence() const noexcept = 0;

public:
    virtual bool Matches(const std::string& text) const
    {
        NFA nfa = ToNFA();
        return nfa.Accepts(text);
    }

    std::string Brackets(size_t outerPrecedence) const
    {
        if (Precedence() < outerPrecedence)
            return std::format("({})", ToString());

        return ToString();
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Regex