#include "Parser.h"
#include "ParseTree.h"

using namespace Regex;

// ---------------------------------------------------------------------------------------------------------------------
// <expr> ::= <term> | <term>'|'<expr>
// <term> ::= <factor> | <factor><term>
// <factor> ::= <atom> | <atom><meta-char>
// <atom> ::= <symbol> | '('<expr>')'
// <symbol> ::= <any-char-except-meta> | '\'<any-char>
// <meta-char> ::= '?' | '*' | '+'
// ---------------------------------------------------------------------------------------------------------------------

auto Parser::Parse() -> NodePtr
{
    auto expr = Expr();
    return expr;
}

// ---------------------------------------------------------------------------------------------------------------------
// <expr> ::= <term> | <term>'|'<expr>
// ---------------------------------------------------------------------------------------------------------------------

auto Parser::Expr() -> NodePtr
{
    if (Check('|'))
    {
        auto emptyTerm = EmptyNode::Make();

        Advance();
        auto expr = Expr();

        NodeList children = {
            emptyTerm,
            SymbolNode::Make('|'),
            expr
        };

        return ExprNode::Make(std::move(children));
    }

    auto termOrEmpty = Term();

    if (!CheckNext('|'))
    {
        return ExprNode::Make(std::move(termOrEmpty));
    }

    Advance();
    Advance();

    auto expr = Expr();

    NodeList children = {
        termOrEmpty,
        SymbolNode::Make('|'),
        expr
    };

    return ExprNode::Make(std::move(children));
}

// ---------------------------------------------------------------------------------------------------------------------
// <term> ::= <factor> | <factor><term>
// ---------------------------------------------------------------------------------------------------------------------

auto Parser::Term() -> NodePtr
{
    auto factor = Factor();
    auto token = PeekNext();

    if (token.IsEndToken() || token.IsAnyOf(")|"))
    {
        return TermNode::Make(factor);
    }

    Advance();
    auto term = Term();

    NodeList children = {
        factor,
        term
    };

    return TermNode::Make(std::move(children));
}

// ---------------------------------------------------------------------------------------------------------------------
// <factor> ::= <atom> | <atom><meta-char>
// ---------------------------------------------------------------------------------------------------------------------

auto Parser::Factor() -> NodePtr
{
    auto atom = Atom();
    auto token = PeekNext();

    if (!token.IsMetaChar())
    {
        return FactorNode::Make(std::move(atom));
    }

    Advance();

    NodeList children = {
        atom,
        SymbolNode::Make(token)
    };

    return FactorNode::Make(std::move(children));
}


// ---------------------------------------------------------------------------------------------------------------------
// <atom> ::= <symbol> | '('<expr>')'
// ---------------------------------------------------------------------------------------------------------------------

auto Parser::Atom() -> NodePtr
{
    if (!Check('('))
    {
        auto symbol = Symbol();
        return AtomNode::Make(symbol);
    }

    Advance();
    auto expr = Expr();
    Advance();

    if (!Check(')'))
    {
        throw std::runtime_error(ErrorMsg());
    }

    NodeList children = {
        SymbolNode::Make('('),
        expr,
        SymbolNode::Make(')')
    };

    return AtomNode::Make(std::move(children));
}

// ---------------------------------------------------------------------------------------------------------------------
// <symbol> ::= <any-char-except-meta> | '\'<any-char>
// <meta-char> ::= '?' | '*' | '+'
// ---------------------------------------------------------------------------------------------------------------------

auto Parser::Symbol() -> NodePtr
{
    auto token = Peek();

    if (token.IsMetaChar())
    {
        throw std::runtime_error(ErrorMsg("meta char"));
    }

    auto symbolNode = SymbolNode::Make(token);
    return symbolNode;
}

// ---------------------------------------------------------------------------------------------------------------------

bool Parser::IsAtEnd() const noexcept
{
    return current_ >= tokens_.size();
}

Token Parser::Advance()
{
    if (IsAtEnd())
        return Token::EndOfInput();

    return tokens_.at(++current_);
}

bool Parser::Check(char expected) const
{
    return Peek() == expected;
}

bool Parser::CheckNext(char expected) const
{
    return PeekNext() == expected;
}

Token Parser::Peek() const
{
    if (IsAtEnd())
        return Token::EndOfInput();

    return tokens_.at(current_);
}

Token Parser::PeekNext() const
{
    if (current_ + 1 >= tokens_.size())
        return Token::EndOfInput();

    return tokens_.at(current_ + 1);
}

// ---------------------------------------------------------------------------------------------------------------------

std::string Parser::ErrorMsg(const std::string& tokenType) const
{
    return std::format
    (
        "Error: Unexpected {} {} at position {}.",
        tokenType,
        Peek().Symbol(),
        current_
    );
}