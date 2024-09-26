//
// Created by kemal on 1/28/2024.
//

#ifndef BAKLAVA_TOKENTYPE_H
#define BAKLAVA_TOKENTYPE_H

#include <cstdint>

/**
 * @brief Holds all types of tokens that are valid.
 */
enum class TokenType : uint8_t {
    // Characters
    OpenParenthesis,
    ClosedParenthesis,
    OpenBrace,
    ClosedBrace,
    OpenBracket,
    ClosedBracket,
    Semicolon,
    Colon,
    Comma,
    Dot,
    // Operators
    Plus,
    DoublePlus,
    Minus,
    DoubleMinus,
    Star,
    Slash,
    Percent,
    Bang,
    BangEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Equal,
    DoubleEqual,
    DoublePipe,
    DoubleAmpersand,
    // Types and identifiers
    Identifier,
    Number,
    String,
    True,
    False,
    Null,
    // Keywords
    Print,
    If,
    Else,
    Then,
    Is,
    Var,
    Const,
    While,
    For,
    Function,
    Return,

    // End of file
    Eof
};

#endif //BAKLAVA_TOKENTYPE_H
