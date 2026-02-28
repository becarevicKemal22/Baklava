//
// Created by kemal on 3/14/2024.
//

#ifndef BAKLAVA_UNDECLAREDIDENTIFIER_H
#define BAKLAVA_UNDECLAREDIDENTIFIER_H

#include "RuntimeError.h"
#include "Token.h"

class UndeclaredIdentifier : public RuntimeError {
public:
    explicit UndeclaredIdentifier(Token* name) : RuntimeError(ERROR_UNDECLARED_IDENTIFIER), name(name) {
        messageArguments.push_back(name->value);
    }

    Token* name;
};

#endif //BAKLAVA_UNDECLAREDIDENTIFIER_H
