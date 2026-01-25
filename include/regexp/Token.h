#pragma once
#include <algorithm>
#include <format>
#include <set>

namespace Regex
{
// ---------------------------------------------------------------------------------------------------------------------

enum class TokenType
{
    Symbol,
    MetaChar,
    Empty,
    EndOfInput,
    Error
};

// ---------------------------------------------------------------------------------------------------------------------

class Token
{
    TokenType type_ = TokenType::Empty;
    char symbol_ = 0;
    bool isEscaped_ = false;

private:
    Token() = default;

    explicit Token(const char symbol, bool isEscaped = false)
        : type_(DetectType(symbol))
        , symbol_(symbol)
        , isEscaped_(isEscaped)
    {}

private:
    static TokenType DetectType(char ch)
    {
        using enum TokenType;

        const static std::set<char> kMetaChars = { '*', '+', '?' };

        if (ch == 0)
            return Empty;

        if (ch == -1)
            return EndOfInput;

        if (kMetaChars.contains(ch))
            return MetaChar;

        if (std::islower(ch))
            return Symbol;

        if (std::isupper(ch))
            return Symbol;

        if (std::isdigit(ch))
            return Symbol;

        if (std::ispunct(ch))
            return Symbol;

        return Error;
    }

public:
    bool operator==(char ch) const noexcept
    {
        return symbol_ == ch;
    }

    bool operator!=(char ch) const noexcept
    {
        return symbol_ != ch;
    }

public:
    TokenType Type() const noexcept
    {
        return type_;
    }

    char Symbol() const noexcept
    {
        return symbol_;
    }

    bool IsLiteral() const noexcept
    {
        return !IsMetaChar() && !IsEndToken();
    }

    bool IsEscaped() const noexcept
    {
        return isEscaped_;
    }

    bool IsMetaChar() const noexcept
    {
        const static std::set<char> kMetaChars = { '*', '+', '?' };

        if (IsEscaped())
            return false;

        return kMetaChars.contains(symbol_);
    }

    bool IsEmpty() const noexcept
    {
        return symbol_ == 0;
    }

    bool IsEndToken() const noexcept
    {
        return type_ == TokenType::EndOfInput;
    }

    bool IsAnyOf(std::string_view symbols) const
    {
        return std::ranges::any_of(symbols, [this](char ch)
        {
            return ch == symbol_;
        });
    }

    bool IsNoneOf(std::string_view symbols) const
    {
        return std::ranges::none_of(symbols, [this](char ch)
        {
            return ch == symbol_;
        });
    }

public:
    static Token FromChar(char ch, bool isEscaped = false)
    {
        return Token(ch, isEscaped);
    }

    static Token Empty()
    {
        return Token();
    }

    static Token EndOfInput()
    {
        return Token(-1);
    }

    static Token Error(size_t position)
    {
        Token result(0);
        result.type_ = TokenType::Error;

        return result;
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Regex