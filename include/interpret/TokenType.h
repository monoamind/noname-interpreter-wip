#pragma once

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

enum class TokenType
{
    And,
    Asterisk,
    BraceLeft,
    BraceRight,
    Equal,
    EqualEqual,
    False,
    Identifier,
    Integer,
    Keyword,
    Less,
    LessEqual,
    Minus,
    More,
    MoreEqual,
    Not,
    NotEqual,
    Nil,
    Number,
    Or,
    ParenLeft,
    ParenRight,
    Plus,
    Semicolon,
    Slash,
    String,
    True,
    Eof,
    Error,
    Empty
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Interpret