//
// Created by kemal on 1/28/2024.
//

#ifndef BAKLAVA_ERRORCODE_H
#define BAKLAVA_ERRORCODE_H

/**
 * @brief Standardized error codes for the interpreter that should be used by the entire project.
 *
 * Every single possible error is included in the enum, and no error reporting should be done without using the correct code.
 * 1xx are general errors.
 * 2xx are lexer errors.
 * 3xx are parsing errors.
 * 4xx are interpreter runtime errors.
 * @see ErrorMessages.h for the error messages corresponding to each code.
 */
enum ErrorCode {
    ERROR_CANNOT_OPEN_FILE = 100,

    ERROR_UNEXPECTED_CHARACTER = 200,
    ERROR_UNTERMINATED_STRING,

    ERROR_EXPECTED_X_BEFORE_Y = 300,
    ERROR_UNINITIALIZED_CONST,
    ERROR_EXPECTED_X_AFTER_Y,
    ERROR_IDENTIFIER_REDECLARATION,
    ERROR_UNDECLARED_IDENTIFIER,
    ERROR_CONST_REASSIGNMENT,
    ERROR_EXPECTED_EXPRESSION_AT_START,
    ERROR_INVALID_LVALUE,
    ERROR_INVALID_CALL,
    ERROR_TOO_MANY_ARGUMENTS,
    ERROR_TOO_FEW_ARGUMENTS,
    ERROR_INVALID_RETURN_POSITION,
    ERROR_INVALID_THIS_POSITION,
    ERROR_SELF_REFERENCING_INITIALIZER,
    ERROR_INVALID_DEFAULT_PARAMETER_POSITION,
    ERROR_INVALID_DEFAULT_PARAMETER_VALUE,
    ERROR_INVALID_FOR_LOOP_STEP,
    ERROR_INVALID_NEW,

    ERROR_WRONG_TYPE_OPERAND = 400,
    ERROR_WRONG_TYPE_OPERAND_BINARY,
    ERROR_INDEX_OUT_OF_BOUNDS,
    ERROR_NON_INTEGER_INDEX,
    ERROR_INDEXING_NON_ARRAY,
    ERROR_WRONG_TYPE_TO_STATEMENT,
    ERROR_CONSTRUCTOR_NO_NEW,
    ERROR_CLASS_NOT_FOUND,
    ERROR_INVALID_PROPERTY_ACCESS,
    ERROR_OBJ_HAS_NO_ATTR,
};

#endif //BAKLAVA_ERRORCODE_H
