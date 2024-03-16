//
// Created by kemal on 3/14/2024.
//

#ifndef MATURSKI_2_VARIABLEREDECLARATION_H
#define MATURSKI_2_VARIABLEREDECLARATION_H

#include "RuntimeError.h"
#include "Token.h"

class VariableRedeclaration : public RuntimeError {
public:
    explicit VariableRedeclaration(TokenPtr token) : RuntimeError(ERROR_VARIABLE_REDCLARATION), token(token) {
        messageArguments.push_back(token->value);
    }
    TokenPtr token;
};

#endif //MATURSKI_2_VARIABLEREDECLARATION_H
