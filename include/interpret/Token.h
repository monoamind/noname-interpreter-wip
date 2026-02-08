#pragma once
#include "Object.h"
#include "TokenType.h"

#include <charconv>

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Token
{
    TokenType type_ = TokenType::Empty;
    size_t line_ = 0;
    std::string lexeme_ = {};
    std::optional<Object> literal_ = {};

public:
    Token() = default;

    Token(TokenType type, size_t line, const std::string& lexeme = {})
        : type_(type)
        , line_(line)
        , lexeme_(std::move(lexeme))
    {}

public:
    TokenType Type() const noexcept
    {
        return type_;
    }

    size_t Line() const noexcept
    {
        return line_;
    }

    std::string Lexeme() const
    {
        return lexeme_;
    }

    std::optional<Object> Literal() const
    {
        return literal_;
    }

    void SetLiteral(const Object& value)
    {
        literal_ = value;
    }

public:
    bool HasLexeme(const std::string& lexeme) const
    {
        return lexeme == lexeme_;
    }

    template <typename... Args>
    requires (std::is_same_v<TokenType, Args> || ...)
    bool IsOfType(Args... types) const noexcept
    {
        return ((type_ == types) || ...);
    }

    bool IsError() const noexcept
    {
        return type_ == TokenType::Error;
    }

    bool IsEof() const noexcept
    {
        return type_ == TokenType::Eof;
    }

public:
    static Token Error(size_t line)
    {
        return {TokenType::Error, line};
    }

    static Token Eof(size_t line)
    {
        return {TokenType::Eof, line};
    }

    static Token FromNumber(const std::string& str, size_t line)
    {
        Token result = {TokenType::Number, line, str};

        auto from = str.data();
        auto to = from + str.size();

        double value;
        std::from_chars(from, to, value);

        result.literal_ = value;
        return result;
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Interpret