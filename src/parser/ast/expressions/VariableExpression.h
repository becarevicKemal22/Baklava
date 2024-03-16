//
// Created by kemal on 3/13/2024.
//

#ifndef MATURSKI_2_VARIABLEEXPRESSION_H
#define MATURSKI_2_VARIABLEEXPRESSION_H

#include "Expression.h"
#include "Token.h"

/**
 * @brief Represents a variable expression.
 */
class VariableExpression : public Expression {
public:
    VariableExpression(TokenPtr name) : name(name) {
        type = AstNodeType::VariableExpression;
    }
    TokenPtr name;
};

#endif //MATURSKI_2_VARIABLEEXPRESSION_H
