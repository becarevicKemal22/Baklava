//
// Created by kemal on 11/11/2023.
//

#ifndef MATURSKI_RUNTIMEVALUE_H
#define MATURSKI_RUNTIMEVALUE_H

#include <string>
#include <memory>
#include "ValueType.h"

class RuntimeValue {
public:
    RuntimeValue() = default;
    virtual ~RuntimeValue() = default;
    virtual std::wstring stringify() = 0;
    ValueType type;
};

typedef std::shared_ptr<RuntimeValue> RuntimeValuePtr;

#endif //MATURSKI_RUNTIMEVALUE_H
