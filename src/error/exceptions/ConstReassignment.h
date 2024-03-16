//
// Created by kemal on 3/14/2024.
//

#ifndef MATURSKI_2_CONSTREASSIGNMENT_H
#define MATURSKI_2_CONSTREASSIGNMENT_H

#include "RuntimeError.h"
#include "Token.h"

class ConstReassignment : public RuntimeError {
    public:
    explicit ConstReassignment(Token* name) : RuntimeError(ERROR_CONST_REASSIGNMENT), name(name) {
        messageArguments.push_back(name->value);
    }

    Token* name;
};

#endif //MATURSKI_2_CONSTREASSIGNMENT_H
