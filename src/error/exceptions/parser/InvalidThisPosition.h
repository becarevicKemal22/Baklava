//
// Created by kemal on 10-Feb-26.
//

#ifndef INVALIDTHISPOSITION_H
#define INVALIDTHISPOSITION_H

#include "ParserError.h"
#include "Token.h"

/**
 * @brief Represents an error where 'this' was referenced outside of a class method.
 */
class InvalidThisPosition : public ParserError {
public:
    InvalidThisPosition(TokenPtr token) : ParserError(ERROR_INVALID_THIS_POSITION), token(token) {
        messageArguments.push_back(token->value);
    }

    TokenPtr token;
};

#endif //INVALIDTHISPOSITION_H
