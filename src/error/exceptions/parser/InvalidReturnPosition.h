//
// Created by kemal on 3/27/2024.
//

#ifndef BAKLAVA_INVALIDRETURNPOSITION_H
#define BAKLAVA_INVALIDRETURNPOSITION_H

#include "ParserError.h"
#include "Token.h"

/**
 * @brief Represents an error where a return statement was placed outside of a function body.
 */
class InvalidReturnPosition : public ParserError {
public:
    InvalidReturnPosition(TokenPtr token) : ParserError(ERROR_INVALID_RETURN_POSITION), token(token) {
        messageArguments.push_back(token->value);
    }

    TokenPtr token;
};

#endif //BAKLAVA_INVALIDRETURNPOSITION_H
