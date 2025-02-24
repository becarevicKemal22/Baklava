//
// Created by kemal on 23-Feb-25.
//

#ifndef BAKLAVA_LEXERERROR_H
#define BAKLAVA_LEXERERROR_H

#include <exception>
#include <utility>
#include <vector>

#include "ErrorCode.h"
#include "ErrorMessageArgument.h"

/**
 * @brief Base class for all lexer exceptions.
 *
 * Contains the error code and the message arguments that are used to format the error message, as well as the line and column used for highlighting.
 */
class LexerError : public std::exception {
public:
    LexerError(ErrorCode code, std::vector<ErrorMessageArgument> messageArguments) : code(code),
                                                                                     messageArguments(std::move(
                                                                                             messageArguments)) {}

    ErrorCode code;
    std::vector<ErrorMessageArgument> messageArguments;
    unsigned int line;
    unsigned int column;
};

#endif //BAKLAVA_LEXERERROR_H
