//
// Created by kemal on 16-Feb-25.
//

#ifndef BAKLAVA_INVALIDFORLOOPSTEP_H
#define BAKLAVA_INVALIDFORLOOPSTEP_H

#include "ParserError.h"
#include "Token.h"

/**
 * @brief Represents an error where something other than a numeric literal or a unary expression tied to a numeric literal has been used as for loop step.
 */
class InvalidForLoopStep : public ParserError {
public:
    InvalidForLoopStep(TokenPtr token) : ParserError(
            ERROR_INVALID_FOR_LOOP_STEP), token(std::move(token)) {
    }
    TokenPtr token;

};
#endif //BAKLAVA_INVALIDFORLOOPSTEP_H
