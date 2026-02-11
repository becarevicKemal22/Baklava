//
// Created by kemal on 3/14/2024.
//

#ifndef BAKLAVA_IDENTIFIERREDECLARATION_H
#define BAKLAVA_IDENTIFIERREDECLARATION_H

#include "RuntimeError.h"
#include "Token.h"

class IdentifierRedeclaration : public RuntimeError {
public:
    explicit IdentifierRedeclaration(TokenPtr token) : RuntimeError(ERROR_IDENTIFIER_REDECLARATION), token(token) {
        messageArguments.push_back(token->value);
    }
    TokenPtr token;
};

#endif //BAKLAVA_IDENTIFIERREDECLARATION_H
