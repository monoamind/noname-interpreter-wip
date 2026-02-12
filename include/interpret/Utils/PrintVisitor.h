#pragma once
#include <sstream>
#include <string>

#include "ExpressionAst.h"
#include "Token.h"
#include "Visitor.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class PrintVisitor : ExprVisitor
{
    size_t offset_ = 0;
    std::ostringstream oss_;

public:
    ~PrintVisitor() override = default;

public:
    void Print(const ExprAst& expr);
    std::string Text() const;

    void Visit(const BinaryExpr* expr) override;
    void Visit(const GroupingExpr* expr) override;
    void Visit(const LiteralExpr* expr) override;
    void Visit(const LogicalExpr* expr) override;
    void Visit(const UnaryExpr* expr) override;

private:
    void PrintHeader(const std::string& title);
    void PrintField(const std::string& name, const Token& token);
    void PrintField(const std::string& name, const std::string& str);
    void PrintFooter();

    std::string Indent() const;
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Interpret