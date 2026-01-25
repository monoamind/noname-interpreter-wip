#pragma once
#include <map>
#include <memory>
#include <set>

namespace Regex
{
// ---------------------------------------------------------------------------------------------------------------------

class State;

using Index = size_t;
using IndexSet = std::set<Index>;
using StatePtr = std::shared_ptr<State>;
using StatePtrSet = std::set<StatePtr>;

// ---------------------------------------------------------------------------------------------------------------------

class State
{
    std::map<char, StatePtrSet> transitions_ = {};
    inline const static StatePtrSet kEmptySet = {};

public:
    static StatePtr Make()
    {
        return std::make_shared<State>();
    }

public:
    StatePtrSet& operator[](char symbol)
    {
        return transitions_[symbol];
    }

    const StatePtrSet& At(char symbol) const
    {
        if (transitions_.contains(symbol))
            return kEmptySet;

        return transitions_.at(symbol);
    }

    void AddTransition(char symbol, const StatePtr& state)
    {
        transitions_[symbol].insert(state);
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Regex