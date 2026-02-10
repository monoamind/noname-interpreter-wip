#pragma once

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

// Base.

struct ExprAst;
struct StmtAst;

// ExprAst derived.

struct AssignExpr;
struct BinaryExpr;
struct CallExpr;
struct GroupingExpr;
struct LiteralExpr;
struct LogicalExpr;
struct UnaryExpr;
struct VariableExpr;

// StmtAst derived.

struct BlockStmt;
struct ExpressionStmt;
struct ForStmt;
struct FunctionStmt;
struct IfStmt;
struct PrintStmt;
struct ReturnStmt;
struct VarStmt;
struct WhileStmt;

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core