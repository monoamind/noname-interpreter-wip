#pragma once
#include "Visitor.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

struct ExprAst
{
    virtual void Accept(ExprVisitor& visitor) const = 0;
    virtual ~ExprAst() = default;
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Interpret