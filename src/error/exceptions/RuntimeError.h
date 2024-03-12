//
// Created by kemal on 2/24/2024.
//

#ifndef MATURSKI_2_RUNTIMEERROR_H
#define MATURSKI_2_RUNTIMEERROR_H

#include <exception>
#include <utility>
#include <vector>

#include "ErrorCode.h"
#include "ErrorMessageArgument.h"

/**
 * @brief Base class for all runtime exceptions.
 *
 * Contains at least an error code and a vector of message arguments so it can be printed.
 */
class RuntimeError : public std::exception {
public:
    RuntimeError(ErrorCode code) : code(code) {}

    RuntimeError(ErrorCode code, std::vector<ErrorMessageArgument> messageArguments) : code(code),
                                                                                messageArguments(std::move(messageArguments)) {}

    ErrorCode code;
    std::vector<ErrorMessageArgument> messageArguments;
};

#endif //MATURSKI_2_RUNTIMEERROR_H
