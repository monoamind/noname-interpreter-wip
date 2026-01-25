#include "ParseTree.h"
#include "Ast.h"

#include <cassert>

using namespace Regex;

// ---------------------------------------------------------------------------------------------------------------------
// <Empty> = ''
// ---------------------------------------------------------------------------------------------------------------------

auto EmptyNode::ConvertToAst() const -> AstNodePtr
{
    return EmptyAst::Make();
}

// ---------------------------------------------------------------------------------------------------------------------
// <Symbol> = <NonMetaChar> | "\"<AnyChar>
// <MetaChar> = "?" | "*" | "+"
// ---------------------------------------------------------------------------------------------------------------------

auto SymbolNode::ConvertToAst() const -> AstNodePtr
{
    auto reNode = SymbolAst::Make(token_.Symbol());
    return reNode;
}

// ---------------------------------------------------------------------------------------------------------------------
// <Atom> = <Symbol> | "("<Expr>")"
// ---------------------------------------------------------------------------------------------------------------------

auto AtomNode::ConvertToAst() const -> AstNodePtr
{
    assert(!children_.empty());

    auto symbol = children_[0]->ConvertToAst();

    if (children_.size() > 1)
    {
        assert(symbol->ToString() == "(");

        auto expr = children_[1]->ConvertToAst();
        auto closeParen = children_[2]->ConvertToAst();

        assert(closeParen->ToString() == ")");

        return expr;
    }

    return symbol;
}

// ---------------------------------------------------------------------------------------------------------------------
// <Factor> = <Atom> | <Atom><MetaChar>
// ---------------------------------------------------------------------------------------------------------------------

auto FactorNode::ConvertToAst() const -> AstNodePtr
{
    assert(!children_.empty());

    // Brackets
    auto atom = children_[0]->ConvertToAst();

    if (children_.size() < 2)
    {
        return atom;
    }

    // MetaChar
    auto symbol = children_[1]->ConvertToAst();

    switch (symbol->ToString()[0])
    {
    case '*':
        return RepetitionAst::Make(atom);
    case '?':
        return OptionalAst::Make(atom);
    case '+':
        return OneOrMoreAst::Make(atom);
    default:
        assert(false && "Invalid meta character");
    }

    return atom;
}

// ---------------------------------------------------------------------------------------------------------------------
// <Term> = <Factor> | <Factor><Term>
// ---------------------------------------------------------------------------------------------------------------------

auto TermNode::ConvertToAst() const -> AstNodePtr
{
    assert(!children_.empty());

    if (children_[0]->IsEmpty())
        return EmptyAst::Make();

    auto factor = children_[0]->ConvertToAst();

    if (children_.size() == 2)
    {
        auto term = children_[1]->ConvertToAst();

        auto result = ConcatenationAst::Make(factor, term);
        return result;
    }

    return factor;
}

// ---------------------------------------------------------------------------------------------------------------------
// <Expr> ::= <TermOrEmpty> | <TermOrEmpty>"|"<Expr>
// <TermOrEmpty> ::= <Term> | <Empty>
// ---------------------------------------------------------------------------------------------------------------------

auto ExprNode::ConvertToAst() const -> AstNodePtr
{
    assert(!children_.empty());
    assert(children_.size() != 2);

    auto termOrEmpty = children_[0]->ConvertToAst();

    if (children_.size() == 3)
    {
        auto expr = children_[2]->ConvertToAst();

        auto result = AlternationAst::Make(termOrEmpty, expr);
        return result;
    }

    return termOrEmpty;
}