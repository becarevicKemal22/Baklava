//
// Created by kemal on 3/11/2024.
//

#ifndef BAKLAVA_LOGICALEXPRESSION_H
#define BAKLAVA_LOGICALEXPRESSION_H

#include "Expression.h"
#include "Token.h"

/**
 * @brief Represents a logical binary expression.
 *
 * Used for logical && and ||.
 */
class LogicalExpression : public Expression {
public:
    LogicalExpression(ExprPtr left, Token* op, ExprPtr right) : left(left), op(op), right(right) {
        type = AstNodeType::LogicalExpression;
    }
    ExprPtr left;
    Token* op;
    ExprPtr right;
};

#endif //BAKLAVA_LOGICALEXPRESSION_H
