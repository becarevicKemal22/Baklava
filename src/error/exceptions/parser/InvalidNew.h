//
// Created by kemal on 03-Jul-25.
//

#ifndef INVALIDNEW_H
#define INVALIDNEW_H

#include "ParserError.h"
#include "Object.h"

class InvalidNew : public ParserError {
public:
    InvalidNew(Token* tok) : ParserError(ERROR_INVALID_NEW) {
        messageArguments.emplace_back(tok->value);
        this->token = tok;
    }
    Token* token;
};

#endif //INVALIDNEW_H
