//
// Created by kemal on 11/11/2023.
//

#ifndef MATURSKI_BOOLEANVALUE_H
#define MATURSKI_BOOLEANVALUE_H

#include "RuntimeValue.h"

class BooleanValue : public RuntimeValue {
    public:
    explicit BooleanValue(bool value) : RuntimeValue(), value(value) {
        type = ValueType::Boolean;
    }
    bool value;
    std::wstring stringify() override {
        if(value){
            return L"tacno";
        }else{
            return L"netacno";
        }
    }
};

#endif //MATURSKI_BOOLEANVALUE_H
