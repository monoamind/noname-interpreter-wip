#include "Ast.h"
#include "NFA.h"

using namespace Regex;

// ---------------------------------------------------------------------------------------------------------------------

NFA EmptyAst::ToNFA() const
{
    NFA nfa;
    return nfa;
}

std::string EmptyAst::ToString() const
{
    return {};
}

size_t EmptyAst::Precedence() const noexcept
{
    return 3;
}

auto EmptyAst::Make() -> AstNodePtr
{
    return std::make_shared<EmptyAst>();
}

// ---------------------------------------------------------------------------------------------------------------------

NFA SymbolAst::ToNFA() const
{
    NFA nfa(2);
    nfa.ResetFinalStates({ 1 });
    nfa.AddTransition(0, symbol_, 1);

    return nfa;
}

std::string SymbolAst::ToString() const
{
    return {symbol_};
}

size_t SymbolAst::Precedence() const noexcept
{
    return 3;
}

auto SymbolAst::Make(char symbol) -> AstNodePtr
{
    return std::make_shared<SymbolAst>(symbol);
}

// ---------------------------------------------------------------------------------------------------------------------

NFA AlternationAst::ToNFA() const
{
    auto nfa = patternX_->ToNFA();
    auto startX = 1;
    auto startY = nfa.Size() + 1;

    nfa.PushFront();
    nfa.AddStartStateTransition(NFA::kEpsilon, startX);

    nfa.Merge(patternY_->ToNFA());
    nfa.AddStartStateTransition(NFA::kEpsilon, startY);

    return nfa;
}

std::string AlternationAst::ToString() const
{
    auto result = patternX_->Brackets(Precedence());
    result += '|';
    result += patternY_->Brackets(Precedence());

    return result;
}

size_t AlternationAst::Precedence() const noexcept
{
    return 0;
}

auto AlternationAst::Make(AstNodePtr lhs, AstNodePtr rhs) -> AstNodePtr
{
    return std::make_shared<AlternationAst>(lhs, rhs);
}

// ---------------------------------------------------------------------------------------------------------------------

NFA ConcatenationAst::ToNFA() const
{
    auto nfaX = patternX_->ToNFA();
    auto nfaY = patternY_->ToNFA();

    auto startY = nfaX.Size();
    auto oldFinalStates = nfaX.FinalStates();

    nfaX.ResetFinalStates();
    nfaX.Merge(std::move(nfaY));

    for (auto& state : oldFinalStates)
    {
        state->AddTransition({}, nfaX.At(startY));
    }

    return nfaX;
}

std::string ConcatenationAst::ToString() const
{
    auto result = patternX_->Brackets(Precedence());
    result += patternY_->Brackets(Precedence());

    return result;
}

size_t ConcatenationAst::Precedence() const noexcept
{
    return 1;
}

auto ConcatenationAst::Make(AstNodePtr lhs, AstNodePtr rhs) -> AstNodePtr
{
    return std::make_shared<ConcatenationAst>(lhs, rhs);
}

// ---------------------------------------------------------------------------------------------------------------------

NFA RepetitionAst::ToNFA() const
{
    auto nfa = pattern_->ToNFA();

    nfa.PushFront();
    nfa[0].AddTransition({}, nfa.At(1));
    nfa.AddFinalStates({ 0 });

    for (auto& state : nfa.FinalStates())
    {
        state->AddTransition({}, nfa.At(1));
    }

    return nfa;
}

std::string RepetitionAst::ToString() const
{
    return pattern_->Brackets(Precedence()) + '*';
}

size_t RepetitionAst::Precedence() const noexcept
{
    return 2;
}

auto RepetitionAst::Make(AstNodePtr pattern) -> AstNodePtr
{
    return std::make_shared<RepetitionAst>(pattern);
}

// ---------------------------------------------------------------------------------------------------------------------

NFA OneOrMoreAst::ToNFA() const
{
    auto nfa = pattern_->ToNFA();

    nfa.PushFront();
    nfa[0].AddTransition({}, nfa.At(1));

    for (auto& state : nfa.FinalStates())
    {
        state->AddTransition({}, nfa.At(1));
    }

    return nfa;
}

std::string OneOrMoreAst::ToString() const
{
    return pattern_->Brackets(Precedence()) + '+';
}

size_t OneOrMoreAst::Precedence() const noexcept
{
    return 2;
}

auto OneOrMoreAst::Make(AstNodePtr pattern) -> AstNodePtr
{
    return std::make_shared<OneOrMoreAst>(pattern);
}

// ---------------------------------------------------------------------------------------------------------------------

NFA OptionalAst::ToNFA() const
{
    auto nfa = pattern_->ToNFA();
    nfa.PushFront();
    nfa[0].AddTransition({}, nfa.At(1));

    for (auto& state : nfa.FinalStates())
    {
        nfa[0].AddTransition(0, state);
    }

    return nfa;
}

std::string OptionalAst::ToString() const
{
    return pattern_->Brackets(Precedence()) + '?';
}

size_t OptionalAst::Precedence() const noexcept
{
    return 2;
}

auto OptionalAst::Make(AstNodePtr pattern) -> AstNodePtr
{
    return std::make_shared<OptionalAst>(pattern);
}