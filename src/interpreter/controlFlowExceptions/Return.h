//
// Created by kemal on 3/19/2024.
//

#ifndef MATURSKI_2_RETURN_H
#define MATURSKI_2_RETURN_H

#include "RuntimeValue.h"
#include <exception>

class Return : public std::exception {
public:
    explicit Return(RuntimeValue value) : value(value) {}

    RuntimeValue value;
};

#endif //MATURSKI_2_RETURN_H
