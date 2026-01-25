#pragma once
#include "Token.h"
#include <vector>

namespace Regex
{
// ---------------------------------------------------------------------------------------------------------------------

class Scanner
{
    std::string src_ = {};
    std::vector<Token> tokens_ = {};
    size_t pos_ = 0;

public:
    explicit Scanner(std::string src)
        : src_(std::move(src))
    {}

public:
    const std::vector<Token>& ScanTokens();

private:
    Token ScanToken();
    Token ScanEscaped();

    bool IsAtEnd() const noexcept;
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Regex