//
// Created by kemal on 23-Feb-26.
//

#ifndef BAKLAVA_CONSTRUCTORRETURNWITHVALUE_H
#define BAKLAVA_CONSTRUCTORRETURNWITHVALUE_H

#include "ParserError.h"
#include "Token.h"

/**
 * @brief Represents an error where a return statement with a return value was placed inside a class constructor body. Returns without values are allowed.
 */
class ConstructorReturnWithValue : public ParserError {
public:
    ConstructorReturnWithValue(TokenPtr token) : ParserError(ERROR_CONSTRUCTOR_RETURN_WITH_VALUE), token(token) {
        messageArguments.push_back(token->value);
    }

    TokenPtr token;
};

#endif //BAKLAVA_CONSTRUCTORRETURNWITHVALUE_H
