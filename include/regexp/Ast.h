#pragma once
#include "AstNode.h"

namespace Regex
{
// ---------------------------------------------------------------------------------------------------------------------

class EmptyAst final : public AstNode
{
public:
    ~EmptyAst() override = default;

public:
    NFA ToNFA() const override;
    std::string ToString() const override;
    size_t Precedence() const noexcept override;

public:
    static AstNodePtr Make();
};

// ---------------------------------------------------------------------------------------------------------------------

class SymbolAst final : public AstNode
{
    char symbol_ = {};

public:
    explicit SymbolAst(char symbol)
        : symbol_(symbol)
    {}

    ~SymbolAst() override = default;

public:
    NFA ToNFA() const override;
    std::string ToString() const override;
    size_t Precedence() const noexcept override;

public:
    static AstNodePtr Make(char symbol);
};

// ---------------------------------------------------------------------------------------------------------------------

class AlternationAst final : public AstNode
{
    AstNodePtr patternX_;
    AstNodePtr patternY_;

public:
    AlternationAst(AstNodePtr lhs, AstNodePtr rhs)
        : patternX_(lhs)
        , patternY_(rhs)
    {}

    ~AlternationAst() override = default;

public:
    NFA ToNFA() const override;
    std::string ToString() const override;
    size_t Precedence() const noexcept override;

public:
    static AstNodePtr Make(AstNodePtr lhs, AstNodePtr rhs);
};

// ---------------------------------------------------------------------------------------------------------------------

class ConcatenationAst final : public AstNode
{
    AstNodePtr patternX_;
    AstNodePtr patternY_;

public:
    ConcatenationAst(AstNodePtr lhs, AstNodePtr rhs)
        : patternX_(lhs)
        , patternY_(rhs)
    {}

    ~ConcatenationAst() override = default;

public:
    NFA ToNFA() const override;
    size_t Precedence() const noexcept override;
    std::string ToString() const override;

public:
    static AstNodePtr Make(AstNodePtr lhs, AstNodePtr rhs);
};

// ---------------------------------------------------------------------------------------------------------------------

class RepetitionAst : public AstNode
{
    AstNodePtr pattern_;

public:
    explicit RepetitionAst(AstNodePtr pattern)
        : pattern_(pattern)
    {}

public:
    NFA ToNFA() const override;
    std::string ToString() const override;
    size_t Precedence() const noexcept override;

public:
    static AstNodePtr Make(AstNodePtr pattern);
};

// ---------------------------------------------------------------------------------------------------------------------

class OneOrMoreAst final : public AstNode
{
    AstNodePtr pattern_;

public:
    explicit OneOrMoreAst(AstNodePtr pattern)
        : pattern_(pattern)
    {}

public:
    NFA ToNFA() const override;
    size_t Precedence() const noexcept override;
    std::string ToString() const override;

public:
    static AstNodePtr Make(AstNodePtr pattern);
};

// ---------------------------------------------------------------------------------------------------------------------

class OptionalAst final : public AstNode
{
    AstNodePtr pattern_;

public:
    explicit OptionalAst(AstNodePtr pattern)
        : pattern_(pattern)
    {}

public:
    NFA ToNFA() const override;
    std::string ToString() const override;
    size_t Precedence() const noexcept override;

public:
    static AstNodePtr Make(AstNodePtr pattern);
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Regex