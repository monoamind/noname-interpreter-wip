#include "Scanner.h"
#include "Runner.h"

#include <regexp/RegExp.h>

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<Token>& Scanner::ScanTokens()
{
    while (!IsAtEnd())
    {
        ScanToken();
    }

    tokens_.push_back(Token::EndOfFile(line_));
    return tokens_;
}

// ---------------------------------------------------------------------------------------------------------------------

void Scanner::ScanToken()
{
    using enum TokenType;

    while (std::isspace(Peek()))
    {
        if (Peek() == '\n')
            ++line_;
        Advance();
    }

    if (Peek() == '"')
    {
        Advance();
        AddString();
        return;
    }

    if (std::isdigit(Peek()))
    {
        AddNumber();
        return;
    }

    if (std::isalpha(Peek()) || Peek() == '_')
    {
        AddIdentifier();
        return;
    }

    if (Peek() == EOF)
    {
        Advance();
        return;
    }

    Token token = Token::FromSymbol(Peek(), line_);
    Advance();

    if (token.IsOfType(More, Less, Not, Equal))
    {
        if (Peek() == '=')
        {
            Advance();
            std::string twoSymbols = token.Lexeme() + '=';
            token = Token::FromTwoSymbols(twoSymbols, line_);
        }

        tokens_.emplace_back(token);
    }
    else if (token.IsError())
    {
        Runner::Error(line_, std::format("Unexpected character: {}.", Peek()));
    }
    else
    {
        tokens_.emplace_back(token);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void Scanner::AddIdentifier()
{
    Regex::RegExp regexp("[A-Za-z_][A-Za-z0-9_]*");
    auto str = regexp.LongestMatch(source_, pos_);
    Token token;

    if (kwMap.contains(str))
    {
        token = Token::FromKeyword(kwMap.at(str), str, line_);
    }
    else
    {
        token = Token::FromIdentifier(str, line_);
    }

    tokens_.emplace_back(token);
    pos_ += str.size();
}

void Scanner::AddNumber()
{
    Regex::RegExp regexp("[0-9]+.?[0-9]*");

    auto numStr = regexp.LongestMatch(source_, pos_);
    pos_ += numStr.size();

    tokens_.emplace_back(Token::FromNumber(numStr, line_));
}

void Scanner::AddString()
{
    std::string str;

    while (Peek() != '"' && !IsAtEnd())
    {
        str += Peek();
        Advance();
    }

    Advance();
    tokens_.emplace_back(Token::FromString(str, line_));
}