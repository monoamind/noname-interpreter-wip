#include "Rewriter.h"

using namespace Regex;

// ---------------------------------------------------------------------------------------------------------------------

void Rewriter::Rewrite()
{
    while (!src_.empty())
    {
        if (src_[0] == '\\')
        {
            RewriteEscape(src_[1]);
            src_.remove_prefix(2);
            continue;
        }

        if (src_[0] != '[')
        {
            result_ += src_[0];
            src_.remove_prefix(1);
            continue;
        }

        auto to = 1;
        bool slash = false;

        while (src_[to] != ']' || slash)
        {
            if (src_[to++] == '\\')
            {
                slash = !slash;
                continue;
            }

            slash = false;
        }

        RewriteClass(src_.substr(1, to - 1));
        src_.remove_prefix(to + 1);
    }
}

void Rewriter::RewriteClass(std::string_view cls)
{
    assert(!cls.empty());

    result_ += '(';

    char prev = 0;
    char cur = 0;

    while (!cls.empty())
    {
        cur = cls[0];

        if (cur == '\\')
        {
            if (prev != 0)
                result_ += std::format("{}|", prev);

            prev = cls[1];
            cls.remove_prefix(2);
            continue;
        }

        if (cur != '-')
        {
            if (prev != 0)
                result_ += std::format("{}|", prev);

            prev = cur;
            cls.remove_prefix(1);
            continue;
        }

        cls.remove_prefix(1);
        cur = cls[0];

        if (cur == '\\')
        {
            cls.remove_prefix(1);
            cur = cls[0];
        }

        RewriteRange(prev, cur);
        cls.remove_prefix(1);
        prev = 0;
    }

    if (prev != 0)
        result_ += std::format("{}|", prev);

    if (result_.back() == '|')
        result_.pop_back();

    result_ += ')';
}

void Rewriter::RewriteEscape(char ch)
{
    constexpr static std::string kWhitespace = "nrtfv";

    switch (ch)
    {
        case 'w':
            RewriteClass("A-Za-z0-9_");
            return;
        case 'd':
            RewriteClass("0-9");
            return;
        case 's':
            RewriteClass(R"( \n\r\t\f\v)");
            return;
        default:
            break;
    }

    if (kWhitespace.find(ch) != std::string::npos)
        result_ += '\\';

    result_ += ch;
}

void Rewriter::RewriteRange(char from, char to)
{
    assert(from <= to);

    while (from <= to)
        result_ += std::format("{}|", from++);
}