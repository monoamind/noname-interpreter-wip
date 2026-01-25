#include "NFA.h"

#include <algorithm>
#include <deque>

using namespace Regex;

// ---------------------------------------------------------------------------------------------------------------------
// NFA::Impl declaration.
// ---------------------------------------------------------------------------------------------------------------------

struct NFA::Impl
{
    bool Accepts(const std::string& str);
    MatchList FindMatches(const std::string& str);
    std::string LongestMatch(const std::string& str, size_t pos = 0);
    size_t FindLongestMatchImpl(std::string_view strView);

    void ReadCharacter(char symbol);
    void ReadString(const std::string& str);

    bool IsInFinalState() const;
    bool IsInFailedState() const;
    void ResetCurrentStates();

    StatePtrSet AllPossibleCurrentStates(StatePtrSet&& states) const;
    StatePtrSet NextStates(const StatePtrSet& states, char symbol) const;
    StatePtrSet NextFreeMoveStates(const StatePtrSet& states) const;

public:
    std::deque<StatePtr> states_ = {};
    StatePtrSet currentStates_ = {};
    StatePtrSet finalStates_ = {};
};

// ---------------------------------------------------------------------------------------------------------------------
// NFA class implementation.
// ---------------------------------------------------------------------------------------------------------------------

NFA::NFA(size_t stateCount)
    : impl_(std::make_shared<Impl>())
{
    if (stateCount == 0)
        return;

    impl_->states_.resize(stateCount);

    for (size_t id = 0; id < stateCount; ++id)
        impl_->states_[id] = std::make_shared<State>();

    ResetFinalStates({0});
}

NFA::~NFA() = default;

State& NFA::operator[](Index idx)
{
    auto& states = impl_->states_;

    if (states.size() <= idx)
        throw std::invalid_argument("No state with the index of " + std::to_string(idx));

    return *states[idx];
}

const StatePtr& NFA::At(Index idx) const
{
    auto& states = impl_->states_;

    if (states.size() <= idx)
        throw std::invalid_argument("No state with the index of " + std::to_string(idx));

    return states.at(idx);
}

bool NFA::Accepts(const std::string& str) const
{
    impl_->ResetCurrentStates();
    impl_->ReadString(str);

    return impl_->IsInFinalState();
}

auto NFA::FindMatches(const std::string& str) const -> MatchList
{
    return impl_->FindMatches(str);
}

std::string NFA::LongestMatch(const std::string& str, size_t pos) const
{
    std::string_view strView = str;
    strView = strView.substr(pos);

    if (size_t longest = impl_->FindLongestMatchImpl(strView); longest > 0)
    {
        return str.substr(pos, longest);
    }

    return {};
}

size_t NFA::Size() const noexcept
{
    return impl_->states_.size();
}

const StatePtrSet& NFA::CurrentStates() const
{
    return impl_->currentStates_;
}

const StatePtrSet& NFA::FinalStates() const
{
    return impl_->finalStates_;
}

void NFA::AddTransition(Index from_idx, char symbol, Index to_idx)
{
    auto& states = impl_->states_;

    auto& from = *states[from_idx];
    auto to = states[to_idx];

    from[symbol].insert(to);
}

void NFA::AddEpsilonTransition(Index from_idx, Index to_idx)
{
    return AddTransition(from_idx, kEpsilon, to_idx);
}

void NFA::AddStartStateTransition(char symbol, Index to_idx)
{
    return AddTransition(0, symbol, to_idx);
}

void NFA::AddFinalStates(const IndexSet& indices)
{
    for (auto idx : indices)
        impl_->finalStates_.insert(impl_->states_[idx]);
}

void NFA::ResetFinalStates(const IndexSet& indices)
{
    impl_->finalStates_.clear();
    AddFinalStates(indices);
}

void NFA::PushFront()
{
    impl_->states_.emplace_front(State::Make());
    AddTransition(0, kEpsilon, 1);
}

void NFA::PushBack()
{
    impl_->states_.emplace_back(State::Make());
}

void NFA::Merge(NFA&& source)
{
    auto& srcImpl = source.impl_;

    impl_->ResetCurrentStates();
    impl_->finalStates_.merge(srcImpl->finalStates_);

    std::ranges::move(srcImpl->states_, std::back_inserter(impl_->states_));
}

// ---------------------------------------------------------------------------------------------------------------------
// NFA::Impl struct implementation.
// ---------------------------------------------------------------------------------------------------------------------

NFA::MatchList NFA::Impl::FindMatches(const std::string& str)
{
    MatchList result;

    std::size_t startPos = 0;
    size_t longest = 0;
    std::string_view strView(str.data(), str.size());

    while (startPos < str.size())
    {
        longest = FindLongestMatchImpl(strView);

        if (longest == 0)
        {
            strView.remove_prefix(1);
            startPos += 1;
            continue;
        }

        result.emplace_back(startPos, std::string(str, startPos, longest));
        strView.remove_prefix(longest);
        startPos += longest;
    }

    return result;
}

std::string NFA::Impl::LongestMatch(const std::string& str, size_t pos)
{
    std::string_view strView(str.data() + pos, str.size() - pos);
    size_t longest = FindLongestMatchImpl(strView);

    if (longest > 0)
    {
        return str.substr(pos, longest);
    }

    return {};
}

void NFA::Impl::ReadCharacter(char symbol)
{
    auto nextStates = NextStates(currentStates_, symbol);
    currentStates_ = AllPossibleCurrentStates(std::move(nextStates));
}

void NFA::Impl::ReadString(const std::string& str)
{
    for (auto ch : str)
    {
        ReadCharacter(ch);
    }
}

bool NFA::Impl::IsInFinalState() const
{
    return std::ranges::any_of(currentStates_, [this](auto& state)
    {
        return finalStates_.contains(state);
    });
}

bool NFA::Impl::IsInFailedState() const
{
    return currentStates_.empty();
}

void NFA::Impl::ResetCurrentStates()
{
    auto& startState = states_[0];
    currentStates_ = AllPossibleCurrentStates({ startState });
}

size_t NFA::Impl::FindLongestMatchImpl(std::string_view strView)
{
    ResetCurrentStates();

    size_t result = 0;
    size_t current = 0;

    for (auto& ch : strView)
    {
        ReadCharacter(ch);

        if (IsInFailedState())
            return result;

        ++current;

        if (IsInFinalState())
        {
            result += current;
            current = 0;
        }
    }

    return result;
}

StatePtrSet NFA::Impl::AllPossibleCurrentStates(StatePtrSet&& states) const
{
    auto currStates = std::move(states);
    auto freeMoveStates = NextFreeMoveStates(currStates);

    if (std::ranges::includes(currStates, freeMoveStates))
    {
        return currStates;
    }

    currStates.merge(freeMoveStates);

    auto result = AllPossibleCurrentStates(std::move(currStates));
    return result;
}

StatePtrSet NFA::Impl::NextFreeMoveStates(const StatePtrSet& states) const
{
    return NextStates(states, kEpsilon);
}

StatePtrSet NFA::Impl::NextStates(const StatePtrSet& curr_states, char symbol) const
{
    StatePtrSet next_states;

    for (auto& state_ptr : curr_states)
    {
        auto& states = (*state_ptr)[symbol];
        next_states.insert(states.begin(), states.end());
    }

    return next_states;
}