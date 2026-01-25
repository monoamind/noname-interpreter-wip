#include "Scanner.h"

using namespace Regex;

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<Token>& Scanner::ScanTokens()
{
    while (!IsAtEnd())
    {
        tokens_.emplace_back(ScanToken());
    }

    tokens_.emplace_back(Token::EndOfInput());
    return tokens_;
}

Token Scanner::ScanToken()
{
    if (char ch = src_[pos_++]; ch != '\\')
    {
        return Token::FromChar(ch);
    }

    return ScanEscaped();
}

Token Scanner::ScanEscaped()
{
    if (IsAtEnd())
        return Token::Error(pos_);

    char ch = src_[pos_++];
    return Token::FromChar(ch, true);
}

bool Scanner::IsAtEnd() const noexcept
{
    return pos_ >= src_.length();
}