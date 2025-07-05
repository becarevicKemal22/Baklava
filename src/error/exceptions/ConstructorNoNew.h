//
// Created by kemal on 02-Jul-25.
//

#ifndef CONSTRUCTORNONEW_H
#define CONSTRUCTORNONEW_H

#include "RuntimeError.h"
#include "ExceptionHelpers.h"
#include "RuntimeValue.h"
#include "Expression.h"
#include "Object.h"


/**
 * @brief Exception thrown when calling class constructor without new keyword.
 */
class ConstructorNoNew : public RuntimeError {
public:
    ConstructorNoNew(Expression* expression, RuntimeValue classObj) : RuntimeError(ErrorCode::ERROR_CONSTRUCTOR_NO_NEW) {
        auto obj = reinterpret_cast<ObjectClass*>(classObj.as.object);
        messageArguments.emplace_back(obj->name);
        this->token = getMostRelevantToken(expression);
    }
    Token* token;
};

#endif //CONSTRUCTORNONEW_H
