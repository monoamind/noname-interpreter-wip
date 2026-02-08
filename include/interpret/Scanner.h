#pragma once
#include "Token.h"
#include <regexp/RegExp.h>

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Scanner
{
    std::string source_ = {};
    std::vector<Token> tokens_ = {};
    size_t pos_ = 0;
    size_t begin_ = 0;
    size_t line_ = 1;

public:
    explicit Scanner(const std::string& source)
        : source_{source}
    {}

public:
    const std::vector<Token>& ScanTokens();

private:
    void ScanToken();

    void AddSymbol(TokenType type);
    void AddNumber();
    void AddString();
    void AddIdentifier();

    bool IsAlpha(char ch) const;
    bool IsAlphaNumeric(char ch) const;
    bool IsAtEnd() const noexcept;
    
    char Advance();
    char Peek() const;
    bool Match(char expected);

private:
    inline const static std::unordered_map<std::string, TokenType> kwMap = {
        {"and",   TokenType::And},
        {"else",  TokenType::Else},
        {"false", TokenType::False},
        {"if",    TokenType::If},
        {"print", TokenType::Print}
    };
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core