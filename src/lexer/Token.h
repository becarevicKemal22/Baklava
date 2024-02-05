//
// Created by kemal on 1/28/2024.
//

#ifndef MATURSKI_2_TOKEN_H
#define MATURSKI_2_TOKEN_H

#include <string>
#include <utility>
#include <memory>
#include "TokenType.h"

class Token {
public:
    Token(TokenType type, const std::wstring& value, unsigned int line, unsigned int offset) : type(type), value(value), line(line), offset(offset) {}
    TokenType type;
    std::wstring value;
    unsigned int line;
    unsigned int offset;
};

typedef Token* TokenPtr;

#endif //MATURSKI_2_TOKEN_H
