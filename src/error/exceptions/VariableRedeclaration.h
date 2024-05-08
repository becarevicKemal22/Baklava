//
// Created by kemal on 3/14/2024.
//

#ifndef BAKLAVA_VARIABLEREDECLARATION_H
#define BAKLAVA_VARIABLEREDECLARATION_H

#include "RuntimeError.h"
#include "Token.h"

class VariableRedeclaration : public RuntimeError {
public:
    explicit VariableRedeclaration(TokenPtr token) : RuntimeError(ERROR_VARIABLE_REDECLARATION), token(token) {
        messageArguments.push_back(token->value);
    }
    TokenPtr token;
};

#endif //BAKLAVA_VARIABLEREDECLARATION_H
