#pragma once
#include "Object.h"
#include "TokenType.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Token
{
    TokenType type_ = TokenType::Empty;
    size_t line_ = 0;
    std::string lexeme_ = {};
    std::optional<Object> literal_ = {};

    Token(TokenType type, size_t line, std::string lexeme = {})
        : type_(type)
        , line_(line)
        , lexeme_(std::move(lexeme))
    {}

public:
    Token() = default;

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

public:
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
    static Token EndOfFile(size_t line);
    static Token FromSymbol(char symbol, size_t line);
    static Token FromTwoSymbols(const std::string& twoSymbols, size_t line);
    static Token FromIdentifier(const std::string& str, size_t line);
    static Token FromKeyword(TokenType kwType, const std::string& kwStr, size_t line);
    static Token FromNumber(const std::string& str, size_t line);
    static Token FromString(const std::string& str, size_t line);
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core