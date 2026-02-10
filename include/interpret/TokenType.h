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
    For,
    Func,
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
    Return,
    Semicolon,
    Slash,
    String,
    True,
    Var,
    While,
    Eof,
    Error,
    Empty
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core