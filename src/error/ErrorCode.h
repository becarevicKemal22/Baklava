//
// Created by kemal on 1/28/2024.
//

#ifndef MATURSKI_2_ERRORCODE_H
#define MATURSKI_2_ERRORCODE_H

/**
 * @brief Standardized error codes for the interpreter that should be used by the entire project.
 *
 * Every single possible error is included in the enum, and no error reporting should be done without using the correct code.
 * 1xx are general errors.
 * 2xx are lexical errors.
 * 3xx are parsing errors.
 * 4xx are interpreter runtime errors.
 * @see ErrorMessages.h for the error messages corresponding to each code.
 */
enum ErrorCode {
    ERROR_CANNOT_OPEN_FILE = 100,

    ERROR_UNEXPECTED_CHARACTER = 200,
    ERROR_UNTERMINATED_STRING = 201,



    ERROR_WRONG_TYPE_OPERAND = 400,
    ERROR_WRONG_TYPE_OPERAND_BINARY = 401,
};

#endif //MATURSKI_2_ERRORCODE_H
