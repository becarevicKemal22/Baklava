//
// Created by kemal on 11/11/2023.
//

#ifndef MATURSKI_VALUETYPE_H
#define MATURSKI_VALUETYPE_H

#include <string>

enum class ValueType {
    Number,
    Boolean,
    String,
    Null,
    Callable,
    Function,
};

std::string stringifyRuntimeType(ValueType type);

#endif //MATURSKI_VALUETYPE_H
