//
// Created by kemal on 9/16/2024.
//

#ifndef BAKLAVA_NATIVEFUNCTION_H
#define BAKLAVA_NATIVEFUNCTION_H

#include "Token.h"
#include "RuntimeValue.h"

struct NativeFunction {
    Token* name;
    RuntimeValue function;
};

Token* generateIdentifierToken(const std::wstring& name);

#endif //BAKLAVA_NATIVEFUNCTION_H
