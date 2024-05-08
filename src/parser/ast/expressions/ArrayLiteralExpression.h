//
// Created by kemal on 5/3/2024.
//

#ifndef BAKLAVA_ARRAYLITERALEXPRESSION_H
#define BAKLAVA_ARRAYLITERALEXPRESSION_H

#include <vector>

#include "Expression.h"
#include "Token.h"

/**
 * @brief Represents an array literal expression.
 */
class ArrayLiteralExpression : public Expression {
public:
    ArrayLiteralExpression(std::vector<ExprPtr> elements, TokenPtr bracket) : elements(std::move(elements)), bracket(std::move(bracket)){
        type = AstNodeType::ArrayLiteralExpression;
    }
    std::vector<ExprPtr> elements;
    TokenPtr bracket;
};

#endif //BAKLAVA_ARRAYLITERALEXPRESSION_H
