//
// Created by kemal on 3/14/2024.
//

#ifndef BAKLAVA_UNDECLAREDVARIABLE_H
#define BAKLAVA_UNDECLAREDVARIABLE_H

#include "RuntimeError.h"
#include "Token.h"

class UndeclaredVariable : public RuntimeError {
public:
    explicit UndeclaredVariable(Token* name) : RuntimeError(ERROR_UNDECLARED_VARIABLE), name(name) {
        messageArguments.push_back(name->value);
    }

    Token* name;
};

#endif //BAKLAVA_UNDECLAREDVARIABLE_H
