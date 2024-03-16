//
// Created by kemal on 3/12/2024.
//

#ifndef MATURSKI_2_PARSERERROR_H
#define MATURSKI_2_PARSERERROR_H

#include <exception>
#include <utility>
#include <vector>

#include "ErrorCode.h"
#include "ErrorMessageArgument.h"

/**
 * @brief Base class for all parser exceptions.
 *
 * Contains at least an error code and a vector of message arguments so it can be printed.
 */
class ParserError : public std::exception {
public:
    ParserError(ErrorCode code) : code(code) {}

    ParserError(ErrorCode code, std::vector<ErrorMessageArgument> messageArguments) : code(code),
                                                                                      messageArguments(std::move(
                                                                                              messageArguments)) {}

    ParserError(ErrorCode code, Token *token) : code(code), myToken(token) {
        messageArguments.push_back(token->value);
    }

    ErrorCode code;
    std::vector<ErrorMessageArgument> messageArguments;
    Token *myToken = nullptr;
};

#endif //MATURSKI_2_PARSERERROR_H
