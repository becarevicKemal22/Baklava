//
// Created by kemal on 3/19/2024.
//

#ifndef BAKLAVA_INVALIDARGUMENTCOUNT_H
#define BAKLAVA_INVALIDARGUMENTCOUNT_H

#include "RuntimeError.h"
#include "Token.h"

class InvalidArgumentCount : public RuntimeError {
public:
    InvalidArgumentCount(int expected, int got, Token *token, Token *paren) : RuntimeError(ERROR_INVALID_ARGUMENT_COUNT),
                                                                             token(token), paren(paren) {
        messageArguments.emplace_back(std::to_wstring(expected));
        messageArguments.emplace_back(std::to_wstring(got));
    }

    Token *token;
    Token *paren;
};

#endif //BAKLAVA_INVALIDARGUMENTCOUNT_H
