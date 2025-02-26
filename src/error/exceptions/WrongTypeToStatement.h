//
// Created by kemal on 26-Feb-25.
//

#ifndef WRONGTYPETOSTATEMENT_H
#define WRONGTYPETOSTATEMENT_H

#include "ExceptionHelpers.h"
#include "RuntimeError.h"
#include "RuntimeValue.h"
#include "Token.h"

class Expression;
/**
 * @brief Exception thrown when an invalid type is given to a statement (e.g. string to "smanji" statement).
 */
class WrongTypeToStatement : public RuntimeError {
public:
    WrongTypeToStatement(Token* keyword, const RuntimeValue& value, const Expression* operand) : RuntimeError(ERROR_WRONG_TYPE_TO_STATEMENT), keyword(keyword) {
        messageArguments.emplace_back(getRuntimeValueTypeString(value));
        messageArguments.emplace_back(keyword->value);
        token = getMostRelevantToken(operand);
    }
    Token *keyword, *token;
};

#endif //WRONGTYPETOSTATEMENT_H
