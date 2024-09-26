//
// Created by kemal on 9/25/2024.
//

#ifndef BAKLAVA_INVALIDDEFAULTPARAMETERPOSITION_H
#define BAKLAVA_INVALIDDEFAULTPARAMETERPOSITION_H

#include "ParserError.h"
#include "Token.h"

/**
 * @brief Represents an error where a default argument appeared before required arguments.
 */
class InvalidDefaultParameterPosition : public ParserError {
public:
    InvalidDefaultParameterPosition(TokenPtr token) : ParserError(
            ERROR_INVALID_DEFAULT_PARAMETER_POSITION), token(std::move(token)) {
    }

    TokenPtr token;

};

#endif //BAKLAVA_INVALIDDEFAULTPARAMETERPOSITION_H
