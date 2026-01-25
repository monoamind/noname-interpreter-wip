#pragma once
#include "AstNode.h"
#include "NFA.h"
#include "Parser.h"
#include "Rewriter.h"
#include "Scanner.h"

#include <cassert>
#include <memory>
#include <vector>

namespace Regex
{
// ---------------------------------------------------------------------------------------------------------------------

class RegExp
{
    std::string pattern_ = {};
    AstNode::AstNodePtr node_ = {};
    NFA nfa_;

public:
    explicit RegExp(std::string_view pattern)
    {
        Initialize(pattern);
    }

public:
    bool Matches(const std::string& str) const
    {
        return nfa_.Accepts(str);
    }

public:
    using Match = NFA::Match;
    using MatchList = std::vector<Match>;

public:
    MatchList FindMatches(const std::string& str) const
    {
        return nfa_.FindMatches(str);
    }

    std::string LongestMatch(const std::string& str, size_t pos = 0) const
    {
        return nfa_.LongestMatch(str, pos);
    }

private:
    void Initialize(std::string_view pattern)
    {
        Rewriter rewriter(pattern);
        pattern_ = rewriter.Result();

        Scanner scanner(pattern_);
        const auto& tokens = scanner.ScanTokens();

        Parser parser(tokens);
        auto parseTree = parser.Parse();
        node_ = parseTree->ConvertToAst();
        nfa_ = std::move(node_->ToNFA());
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Regex;