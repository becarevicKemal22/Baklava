//
// Created by kemal on 9/25/2024.
//

#ifndef BAKLAVA_INVALIDDEFAULTPARAMETERVALUE_H
#define BAKLAVA_INVALIDDEFAULTPARAMETERVALUE_H

#include "ParserError.h"
#include "Token.h"

/**
 * @brief Represents an error where a default parameter is not of a primitive/allowed type.
 */
class InvalidDefaultParameterValue : public ParserError {
public:
    InvalidDefaultParameterValue(TokenPtr token) : ParserError(
            ERROR_INVALID_DEFAULT_PARAMETER_VALUE), token(std::move(token)) {
    }

    TokenPtr token;

};

#endif //BAKLAVA_INVALIDDEFAULTPARAMETERVALUE_H
