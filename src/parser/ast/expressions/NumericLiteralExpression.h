//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_NUMERICLITERALEXPRESSION_H
#define MATURSKI_2_NUMERICLITERALEXPRESSION_H

#include "Expression.h"
#include "Token.h"

/**
 * @brief Represents a numeric literal expression.
 */
class NumericLiteralExpression : public Expression {
public:
    NumericLiteralExpression(TokenPtr token) : token(token) {
        type = AstNodeType::NumericLiteralExpression;
        value = std::stod(token->value);
    }
    TokenPtr token;
    double value;
};

#endif //MATURSKI_2_NUMERICLITERALEXPRESSION_H
