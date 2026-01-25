#pragma once
#include "State.h"

#include <memory>
#include <vector>

namespace Regex
{
// ---------------------------------------------------------------------------------------------------------------------

class NFA
{
    struct Impl;
    std::shared_ptr<Impl> impl_;

public:
    using Match = std::pair<size_t, std::string>;
    using MatchList = std::vector<Match>;

    constexpr static char kEpsilon = '\0';

public:
    explicit NFA(size_t stateCount = 1);
    ~NFA();

    NFA(const NFA& nfaSrc) = delete;
    NFA(NFA&& rhs) noexcept = default;

    NFA& operator=(const NFA& rhs) = delete;
    NFA& operator=(NFA&& rhs) noexcept = default;

public:
    State& operator[](Index idx);
    const StatePtr& At(Index idx) const;

    bool Accepts(const std::string& str) const;
    MatchList FindMatches(const std::string& str) const;
    std::string LongestMatch(const std::string& str, size_t pos = 0) const;

    size_t Size() const noexcept;

    const StatePtrSet& FinalStates() const;
    const StatePtrSet& CurrentStates() const;

    void AddFinalStates(const IndexSet& indices);
    void ResetFinalStates(const IndexSet& indices = {});

    void AddTransition(Index state, char symbol, Index nextState);
    void AddEpsilonTransition(Index state, Index nextState);
    void AddStartStateTransition(char symbol, Index nextState);

    void Merge(NFA&& src);
    void PushBack();
    void PushFront();
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Regex