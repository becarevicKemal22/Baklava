//
// Created by kemal on 03-Jul-25.
//

#ifndef CLASSNOTFOUND_H
#define CLASSNOTFOUND_H

#include "RuntimeError.h"
#include "ExceptionHelpers.h"
#include "RuntimeValue.h"
#include "Expression.h"
#include "Object.h"


/**
 * @brief Exception thrown when new keyword is followed by call expression, but callee is not class name or alias to a class.
 */
class ClassNotFound : public RuntimeError {
public:
    ClassNotFound(Expression* expression) : RuntimeError(ERROR_CLASS_NOT_FOUND) {
        auto tok = getMostRelevantToken(expression);
        messageArguments.emplace_back(tok->value);
        this->token = tok;
    }
    Token* token;
};

#endif //CLASSNOTFOUND_H
