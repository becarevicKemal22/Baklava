//
// Created by kemal on 1/28/2024.
//

#ifndef BAKLAVA_TOKEN_H
#define BAKLAVA_TOKEN_H

#include <string>
#include <utility>
#include <memory>
#include "TokenType.h"

/**
 * @brief Represents a token in the source code.
 */
class Token {
public:
    Token(TokenType type, const std::wstring& value, unsigned int line, unsigned int offset) : type(type), value(value), line(line), offset(offset) {}
    TokenType type;
    std::wstring value;
    unsigned int line; /**< Line on which the token was found. First line is 1 not 0. */
    unsigned int offset; /**< Offset of the first character of the token in the source code. */
};

typedef Token* TokenPtr;

#endif //BAKLAVA_TOKEN_H
