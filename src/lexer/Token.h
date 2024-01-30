//
// Created by kemal on 1/28/2024.
//

#ifndef MATURSKI_2_TOKEN_H
#define MATURSKI_2_TOKEN_H

#include "TokenType.h"

class Token {
public:
    Token(TokenType type, unsigned int line, unsigned int offset) : type(type), line(line), offset(offset) {}
    TokenType type;
    unsigned int line;
    unsigned int offset;
};

#endif //MATURSKI_2_TOKEN_H
