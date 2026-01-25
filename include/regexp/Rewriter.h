#pragma once
#include <cassert>
#include <format>
#include <vector>

namespace Regex
{
// ---------------------------------------------------------------------------------------------------------------------

class Rewriter
{
    std::string result_ = {};
    std::string_view src_ = {};

public:
    explicit Rewriter(std::string_view pattern)
        : src_(pattern)
    {
        if (!src_.empty())
        {
            result_.reserve(src_.size() * 2);
            Rewrite();
        }
    }

public:
    const std::string& Result() const noexcept
    {
        return result_;
    }

private:
    void Rewrite();
    void RewriteClass(std::string_view cls);
    void RewriteEscape(char ch);
    void RewriteRange(char from, char to);
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Regex