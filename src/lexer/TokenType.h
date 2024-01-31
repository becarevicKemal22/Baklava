//
// Created by kemal on 1/28/2024.
//

#ifndef MATURSKI_2_TOKENTYPE_H
#define MATURSKI_2_TOKENTYPE_H

#include <cstdint>

enum class TokenType: uint8_t {
    OpenParenthesis, ClosedParenthesis,
    Identifier, Number,
    // Keywords
    If,
    Eof
};

#endif //MATURSKI_2_TOKENTYPE_H
