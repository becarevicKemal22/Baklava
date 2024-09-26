//
// Created by kemal on 3/19/2024.
//

#ifndef BAKLAVA_TOOMANYARGUMENTS_H
#define BAKLAVA_TOOMANYARGUMENTS_H

#include "RuntimeError.h"
#include "Token.h"

class TooManyArguments : public RuntimeError {
public:
    TooManyArguments(int expected, int got, Token *token, Token *paren) : RuntimeError(ERROR_TOO_MANY_ARGUMENTS),
                                                                          token(token), paren(paren) {
        messageArguments.emplace_back(std::to_wstring(expected));
        messageArguments.emplace_back(std::to_wstring(got));
    }

    Token *token;
    Token *paren;
};

#endif //BAKLAVA_TOOMANYARGUMENTS_H
