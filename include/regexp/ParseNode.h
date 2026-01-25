#pragma once
#include "Token.h"
#include "AstNode.h"

namespace Regex
{
// ---------------------------------------------------------------------------------------------------------------------

class ParseNode
{
public:
    using AstNodePtr = std::shared_ptr<AstNode>;
    using ParseNodePtr = std::shared_ptr<ParseNode>;
    using ChildNodeList = std::vector<ParseNodePtr>;

public:
    enum class NodeType
    {
        Expr,
        Term,
        Factor,
        Atom,
        Symbol,
        Empty
    };

protected:
    NodeType type_;
    ChildNodeList children_ = {};

protected:
    explicit ParseNode(NodeType type)
        : type_(type)
    {}

    ParseNode(NodeType type, const ParseNodePtr& child)
        : type_(type)
        , children_({child})
    {}

    ParseNode(NodeType type, std::vector<ParseNodePtr> children)
        : type_(type)
        , children_(std::move(children))
    {}

    virtual ~ParseNode() = default;

public:
    virtual AstNodePtr ConvertToAst() const = 0;

public:
    virtual bool IsEmpty() const noexcept
    {
        return false;
    }

public:
    ChildNodeList Children() const noexcept
    {
        return children_;
    }

    NodeType Type() const noexcept
    {
        return type_;
    }

public:
    size_t ChildCount() const noexcept
    {
        return children_.size();
    }

    virtual bool IsTerminal() const noexcept
    {
        return children_.empty();
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Regex