#include "Utils/PrintVisitor.h"
#include "ExpressionAst.h"

#include <iostream>

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------

std::string PrintVisitor::Text() const
{
    return oss_.str();
}

void PrintVisitor::Print(const ExprAst& expr)
{
    expr.Accept(*this);
    std::cout << Text() << std::endl;
}

void PrintVisitor::PrintHeader(const std::string& title)
{
    auto indent = Indent();
    oss_ << std::format("\n{}{}", indent, title);
    oss_ << std::format("\n{}(", indent);
    offset_ += 2;
}

void PrintVisitor::PrintField(const std::string& name, const Token& token)
{
    oss_ << std::format("\n{}{}={}", Indent(), name, token.Lexeme());
}

void PrintVisitor::PrintField(const std::string& name, const std::string& str)
{
    oss_ << std::format("\n{}{}={}", Indent(), name, str);
}

void PrintVisitor::PrintFooter()
{
    offset_ -= 2;
    oss_ << std::format("\n{})", Indent());
}

std::string PrintVisitor::Indent() const
{
    std::string indent(offset_, ' ');
    return indent;
}

void PrintVisitor::Visit(const BinaryExpr& expr)
{
    PrintHeader("Binary");
    PrintField("op", expr.operator_);
    expr.left_->Accept(*this);
    expr.right_->Accept(*this);
    PrintFooter();
}

void PrintVisitor::Visit(const GroupingExpr& expr)
{
    PrintHeader("Grouping");
    expr.expr->Accept(*this);
    PrintFooter();
}

void PrintVisitor::Visit(const LiteralExpr& expr)
{
    PrintHeader("Literal");
    PrintField("value", expr.value.ToString());
    PrintFooter();
}

void PrintVisitor::Visit(const LogicalExpr& expr)
{
    PrintHeader("Logical");
    expr.left_->Accept(*this);
    PrintFooter();
}

void PrintVisitor::Visit(const UnaryExpr& expr)
{
    PrintHeader("Unary");
    expr.right_->Accept(*this);
    PrintFooter();
}