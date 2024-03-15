//
// Created by kemal on 3/15/2024.
//

#ifndef MATURSKI_2_INVALIDLVALUE_H
#define MATURSKI_2_INVALIDLVALUE_H

#include "ParserError.h"
#include "Token.h"

/**
 * @brief Represents an error where an invalid lvalue was used.
 */
class InvalidLValue : public ParserError {
public:
    InvalidLValue(TokenPtr token) : ParserError(ERROR_INVALID_LVALUE), token(token) {
        messageArguments.push_back(token->value);
    }

    TokenPtr token;
};

#endif //MATURSKI_2_INVALIDLVALUE_H
