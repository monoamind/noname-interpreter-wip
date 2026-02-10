#include "Scanner.h"
#include "Runner.h"

using namespace Core;


// ---------------------------------------------------------------------------------------------------------------------

const std::vector<Token>& Scanner::ScanTokens()
{
    while (!IsAtEnd())
    {
        ScanToken();
    }

    tokens_.push_back(Token::Eof(line_));
    return tokens_;
}

// ---------------------------------------------------------------------------------------------------------------------

void Scanner::ScanToken()
{
    using enum TokenType;

    char ch = Advance();

    while (std::isspace(ch))
    {
        if (ch == '\n')
            ++line_;
        ch = Advance();
    }

    switch (ch)
    {
        case '(':
            AddSymbol(ParenLeft);
            return;
        case ')':
            AddSymbol(ParenRight);
            return;
        case '{':
            AddSymbol(BraceLeft);
            return;
        case '}':
            AddSymbol(BraceRight);
            return;
        case '-':
            AddSymbol(Minus);
            return;
        case '+':
            AddSymbol(Plus);
            return;
        case '*':
            AddSymbol(Asterisk);
            return;
        case ';':
            AddSymbol(Semicolon);
            return;
        case '!':
            AddSymbol(Match('=') ? NotEqual : Not);
            return;
        case '=':
            AddSymbol(Match('=') ? EqualEqual : Equal);
            return;
        case '<':
            AddSymbol(Match('=') ? LessEqual : Less);
            return;
        case '>':
            AddSymbol(Match('=') ? MoreEqual : More);
            return;
        case '/':
            AddSymbol(Slash);
            return;
        case '\n':
            line_++;
            return;
        case ' ':
        case '\t':
        case '\r':
            return;
        case '"':
            AddString();
            return;
        default:
            pos_--;
            break;
    }

    if (std::isdigit(ch))
    {
        AddNumber();
    }
    else if (std::isalpha(ch) || ch == '_')
    {
        AddIdentifier();
    }
    else
    {
        Runner::Error(line_, "Unexpected character.");
    }
}

void Scanner::AddIdentifier()
{
    Regex::RegExp regexp("[A-Za-z_][A-Za-z0-9_]");
    auto str = regexp.LongestMatch(source_, pos_);

    if (kwMap.contains(str))
    {
        tokens_.emplace_back(kwMap.at(str), line_, str);
    }
    else
    {
        tokens_.emplace_back(TokenType::Identifier, line_, str);
    }
    pos_ += str.size();
}

void Scanner::AddSymbol(TokenType type)
{
    tokens_.emplace_back(type, line_);
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
    char ch = Advance();
    std::string str;

    while (ch != '"' && !IsAtEnd())
    {
        str += ch;

        if (ch == '\n')
            line_++;

        ch = Advance();
    }

    tokens_.emplace_back(Token::FromString(str, line_));
}

char Scanner::Peek() const
{
    if (IsAtEnd())
        return EOF;

    return source_[pos_];
}

char Scanner::Advance()
{
    if (IsAtEnd())
        return EOF;

    ++pos_;
    return source_[pos_ - 1];
}

bool Scanner::Match(char expected)
{
    if (IsAtEnd())
        return false;

    if (Peek() != expected)
        return false;

    ++pos_;
    return true;
}

bool Scanner::IsAtEnd() const noexcept
{
    return pos_ >= source_.size();
}