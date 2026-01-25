#pragma once
#include "ParseNode.h"

namespace Regex
{
// ---------------------------------------------------------------------------------------------------------------------

class EmptyNode : public ParseNode
{
public:
    EmptyNode()
        : ParseNode(NodeType::Empty)
    {}

    ~EmptyNode() override = default;

public:
    AstNodePtr ConvertToAst() const override;

public:
    bool IsEmpty() const noexcept override
    {
        return true;
    }

public:
    static ParseNodePtr Make()
    {
        return std::make_shared<EmptyNode>();
    }
};

// ---------------------------------------------------------------------------------------------------------------------

class SymbolNode : public ParseNode
{
    Token token_;

public:
    explicit SymbolNode(Token token)
        : ParseNode(NodeType::Symbol)
        , token_(token)
    {}

    explicit SymbolNode(char ch)
        : ParseNode(NodeType::Symbol)
        , token_(Token::FromChar(ch))
    {}

public:
    AstNodePtr ConvertToAst() const override;

public:
    template <typename... Args>
    static ParseNodePtr Make(Args&&... args)
    {
        return std::make_shared<SymbolNode>(std::forward<Args>(args)...);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

class AtomNode : public ParseNode
{
public:
    explicit AtomNode(ParseNodePtr child)
        : ParseNode(NodeType::Atom, child)
    {}

    explicit AtomNode(ChildNodeList children)
        : ParseNode(NodeType::Atom, std::move(children))
    {}

    ~AtomNode() override = default;

public:
    AstNodePtr ConvertToAst() const override;

public:
    template <typename... Args>
    static ParseNodePtr Make(Args&&... args)
    {
        return std::make_shared<AtomNode>(std::forward<Args>(args)...);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

class FactorNode : public ParseNode
{
public:
    explicit FactorNode(ParseNodePtr child)
        : ParseNode(NodeType::Factor, child)
    {}

    explicit FactorNode(ChildNodeList children)
        : ParseNode(NodeType::Factor, std::move(children))
    {}

public:
    AstNodePtr ConvertToAst() const override;

public:
    template <typename... Args>
    static ParseNodePtr Make(Args&&... args)
    {
        return std::make_shared<FactorNode>(std::forward<Args>(args)...);
    }
};

// ---------------------------------------------------------------------------------------------------------------------

class TermNode : public ParseNode
{
    using DownType = FactorNode;

public:
    explicit TermNode(ParseNodePtr child)
        : ParseNode(NodeType::Term, child)
    {}

    explicit TermNode(ChildNodeList children)
        : ParseNode(NodeType::Term, std::move(children))
    {}

public:
    AstNodePtr ConvertToAst() const override;

public:
    template <typename... Args>
    static ParseNodePtr Make(Args&&... args)
    {
        return std::make_shared<TermNode>(std::forward<Args>(args)...);
    }
};

// ---------------------------------------------------------------------------------------------------------------------
// <Expr> ::= <TermOrEmpty> | <TermOrEmpty>"|"<Expr>
// <TermOrEmpty> ::= <Term> | <Empty>
// ---------------------------------------------------------------------------------------------------------------------

class ExprNode : public ParseNode
{
    using DownType = ParseNode;

public:
    explicit ExprNode(ParseNodePtr child)
        : ParseNode(NodeType::Expr, child)
    {}

    explicit ExprNode(ChildNodeList children)
        : ParseNode(NodeType::Expr, std::move(children))
    {}

    ~ExprNode() override = default;

public:
    AstNodePtr ConvertToAst() const override;

public:
    template <typename... Args>
    static ParseNodePtr Make(Args&&... args)
    {
        return std::make_shared<ExprNode>(std::forward<Args>(args)...);
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Regex