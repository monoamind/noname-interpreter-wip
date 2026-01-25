#pragma once
#include "ParseNode.h"

#include <memory>
#include <ostream>
#include <vector>

namespace Regex
{
// ---------------------------------------------------------------------------------------------------------------------

class Parser
{
    using NodePtr = ParseNode::ParseNodePtr;
    using NodeList = std::vector<NodePtr>;

private:
    std::vector<Token> tokens_ = {};
    size_t current_ = 0;
    size_t errorPos_ = -1;

public:
    explicit Parser(const std::vector<Token>& tokens)
        : tokens_(tokens)
    {}

public:
    NodePtr Parse();

private:
    NodePtr Expr();
    NodePtr Term();
    NodePtr Factor();
    NodePtr Atom();
    NodePtr Symbol();

private:
    std::string ErrorMsg(const std::string& tokenType = {}) const;

private:
    bool IsAtEnd() const noexcept;
    Token Advance();

    bool Check(char expected) const;
    bool CheckNext(char expected) const;

    Token Peek() const;
    Token PeekNext() const;
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Regex