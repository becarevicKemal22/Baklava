//
// Created by kemal on 9/25/2024.
//

#ifndef BAKLAVA_INVALIDDEFAULTARGUMENTPOSITION_H
#define BAKLAVA_INVALIDDEFAULTARGUMENTPOSITION_H

#include "ParserError.h"
#include "Token.h"

/**
 * @brief Represents an error where a default argument appeared before required arguments.
 */
class InvalidDefaultArgumentPosition : public ParserError {
public:
    InvalidDefaultArgumentPosition(TokenPtr token) : ParserError(
            ERROR_EXPECTED_X_BEFORE_Y), token(std::move(token)) {
    }

    TokenPtr token;

};

#endif //BAKLAVA_INVALIDDEFAULTARGUMENTPOSITION_H
