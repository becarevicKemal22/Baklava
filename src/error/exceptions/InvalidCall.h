//
// Created by kemal on 3/19/2024.
//

#ifndef BAKLAVA_INVALIDCALL_H
#define BAKLAVA_INVALIDCALL_H

#include "RuntimeError.h"
#include "Token.h"
#include "RuntimeValue.h"

class InvalidCall : public RuntimeError {
public:
    explicit InvalidCall(RuntimeValue callee, Token* name) : RuntimeError(ERROR_INVALID_CALL), name(name) {
        messageArguments.push_back(getRuntimeValueTypeString(callee));
    }

    Token* name;
};

#endif //BAKLAVA_INVALIDCALL_H
