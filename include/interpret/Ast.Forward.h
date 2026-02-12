#pragma once

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

struct ExprAst;
struct AssignExpr;
struct BinaryExpr;
struct CallExpr;
struct GroupingExpr;
struct LiteralExpr;
struct LogicalExpr;
struct UnaryExpr;
struct VariableExpr;

// ---------------------------------------------------------------------------------------------------------------------

struct StmtAst;
struct BlockStmt;
struct ExpressionStmt;
struct FunctionStmt;
struct IfStmt;
struct PrintStmt;
struct ReturnStmt;
struct VarStmt;
struct WhileStmt;

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core