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

    switch (ch)
    {
        case '(':
            AddSymbol(ParenLeft);
            break;
        case ')':
            AddSymbol(ParenRight);
            break;
        case '{':
            AddSymbol(BraceLeft);
            break;
        case '}':
            AddSymbol(BraceRight);
            break;
        case '-':
            AddSymbol(Minus);
            break;
        case '+':
            AddSymbol(Plus);
            break;
        case '*':
            AddSymbol(Asterisk);
            break;
        case ';':
            AddSymbol(Semicolon);
            break;
        case '!':
            AddSymbol(Match('=') ? NotEqual : Not);
            break;
        case '=':
            AddSymbol(Match('=') ? EqualEqual : Equal);
            break;
        case '<':
            AddSymbol(Match('=') ? LessEqual : Less);
            break;
        case '>':
            AddSymbol(Match('=') ? MoreEqual : More);
            break;
        case '/':
            AddSymbol(Slash);
            break;
        case '\n':
            line_++;
            break;
        case ' ':
        case '\t':
        case '\r':
            break;
        default:
            pos_--;
            break;
    }

    if (std::isdigit(ch))
    {
        Regex::RegExp regexp("[0-9]+.?[0-9]?");
        auto number_str = regexp.LongestMatch(source_, pos_);

        AddNumber(number_str);
        pos_ += number_str.size();
    }
}

void Scanner::AddSymbol(TokenType type)
{
    tokens_.emplace_back(type, line_);
}

void Scanner::AddNumber(const std::string& numStr)
{
    tokens_.emplace_back(Token::FromNumber(numStr, line_));
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

char Scanner::Peek() const
{
    if (IsAtEnd())
        return EOF;

    return source_[pos_];
}

std::string_view Scanner::CurrentLexeme() const
{
    auto data = source_.data() + begin_;
    auto sz = pos_ - begin_;

    std::string_view sv(data, sz);
    return sv;
}