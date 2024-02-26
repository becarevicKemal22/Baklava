//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_EXPRESSION_H
#define MATURSKI_2_EXPRESSION_H

#include "Statement.h"

/**
 * @brief Base class for all expressions. Not meant to be used aside from achieving polymorphism.
 */
class Expression : public Statement {
public:
    Expression() = default;
    virtual ~Expression() = default;
};

#endif //MATURSKI_2_EXPRESSION_H
