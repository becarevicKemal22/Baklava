//
// Created by kemal on 9/15/2024.
//

#ifndef BAKLAVA_BASEFUNCTIONS_H
#define BAKLAVA_BASEFUNCTIONS_H

#include "Interpreter.h"
#include "NativeFunction.h"

class BaseFunctions {
public:
    static std::vector<NativeFunction> getFunctions(Interpreter* _interpreter);
};

#endif //BAKLAVA_BASEFUNCTIONS_H
