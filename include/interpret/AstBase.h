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

struct StmtAst
{
    virtual void Accept(StmtVisitor& visitor) const = 0;
    virtual ~StmtAst() = default;
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Interpret