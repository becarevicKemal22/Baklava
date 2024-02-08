//
// Created by kemal on 11/11/2023.
//

#ifndef MATURSKI_NULLVALUE_H
#define MATURSKI_NULLVALUE_H

#include "RuntimeValue.h"

class NullValue : public RuntimeValue {
public:
    NullValue() : RuntimeValue() {
        type = ValueType::Null;
    }
    std::wstring stringify() override {
        return L"prazno";
    }
};

#endif //MATURSKI_NULLVALUE_H
