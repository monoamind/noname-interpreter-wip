#include "Common.h"
#include "Token.h"

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------

Token Token::EndOfFile(size_t line)
{
    return {TokenType::Eof, line};
}

Token Token::FromSymbol(char symbol, size_t line)
{
    using enum TokenType;

    static const std::unordered_map<char, TokenType> kSymbolMap = {
        {'(', ParenLeft},  {')', ParenRight}, {'{', BraceLeft},
        {'}', BraceRight}, {'+', Plus},       {'-', Minus},
        {'*', Asterisk},   {'/', Slash},      {';', Semicolon},
        {'!', Not},        {'=', Equal},      {'>', More},
        {'<', Less},       {',', Comma}
    };

    std::string str(1, symbol);

    if (!kSymbolMap.contains(symbol))
    {
        return {Error, line, str};
    }

    return {kSymbolMap.at(symbol), line, str};
}

Token Token::FromTwoSymbols(const std::string& twoSymbols, size_t line)
{
    using enum TokenType;

    static const std::unordered_map<std::string, TokenType> kSymbolMap = {
        {"!=", NotEqual},  {"==", EqualEqual},
        {"<=", LessEqual}, {">=", MoreEqual}
    };

    if (!kSymbolMap.contains(twoSymbols))
    {
        return {Error, line, twoSymbols};
    }

    return {kSymbolMap.at(twoSymbols), line, twoSymbols};
}

Token Token::FromIdentifier(const std::string& id, size_t line)
{
    return {TokenType::Identifier, line, id};
}

Token Token::FromKeyword(TokenType kwType, const std::string& kwStr, size_t line)
{
    return {kwType, line, kwStr};
}

Token Token::FromNumber(const std::string& number, size_t line)
{
    Token result = {TokenType::Number, line, number};

    auto from = number.data();
    auto to = from + number.size();

    double value;
    std::from_chars(from, to, value);

    result.literal_ = value;
    return result;
}

Token Token::FromString(const std::string& str, size_t line)
{
    Token result = {TokenType::String, line, str};

    result.literal_ = str; 
    return result;
}