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
    Else,
    Equal,
    EqualEqual,
    False,
    Identifier,
    If,
    Less,
    LessEqual,
    Minus,
    More,
    MoreEqual,
    Not,
    NotEqual,
    Null,
    Number,
    Or,
    ParenLeft,
    ParenRight,
    Plus,
    Print,
    Semicolon,
    Slash,
    String,
    True,
    Eof,
    Error,
    Empty
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core