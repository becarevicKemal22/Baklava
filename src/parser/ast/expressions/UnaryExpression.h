//
// Created by kemal on 2/5/2024.
//

#ifndef BAKLAVA_UNARYEXPRESSION_H
#define BAKLAVA_UNARYEXPRESSION_H

#include "Expression.h"
#include "Token.h"

/**
 * @brief Represents a unary expression.
 *
 * Includes unary ! and unary -.
 */
class UnaryExpression : public Expression {
public:
    UnaryExpression(Token* op, ExprPtr expr) : op(op), expr(expr) {
        type = AstNodeType::UnaryExpression;
    }
    Token* op;
    ExprPtr expr;
};

#endif //BAKLAVA_UNARYEXPRESSION_H
