#pragma once
#include "Common.h"
#include "Token.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Scanner
{
    std::string source_ = {};
    std::vector<Token> tokens_ = {};
    size_t pos_ = 0;
    size_t line_ = 1;

public:
    explicit Scanner(const std::string& source)
        : source_{source}
    {}

public:
    const std::vector<Token>& ScanTokens();

private:
    void ScanToken();

    void AddNumber();
    void AddString();
    void AddIdentifier();

private:
    bool IsAtEnd() const noexcept
    {
        return pos_ >= source_.size();
    }

    char Peek() const
    {
        if (IsAtEnd())
            return EOF;

        return source_[pos_];
    }

    char Advance()
    {
        if (IsAtEnd())
            return EOF;

        ++pos_;
        return source_[pos_ - 1];
    }

private:
    inline const static std::unordered_map<std::string, TokenType> kwMap = {
        {"and",    TokenType::And},
        {"else",   TokenType::Else},
        {"False",  TokenType::False},
        {"for",    TokenType::For},
        {"func",   TokenType::Func},
        {"if",     TokenType::If},
        {"Null",   TokenType::Null},
        {"or",     TokenType::Or},
        {"print",  TokenType::Print},
        {"return", TokenType::Return},
        {"True",   TokenType::True},
        {"var",    TokenType::Var},
        {"while",  TokenType::While}
    };
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core