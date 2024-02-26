//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_BINARYEXPRESSION_H
#define MATURSKI_2_BINARYEXPRESSION_H

#include "Expression.h"
#include "Ast.h"
#include "Token.h"

/**
 * @brief Represents a binary expression.
 *
 * Includes arithmetic and logical operations.
 */
class BinaryExpression : public Expression {
public:
    BinaryExpression(ExprPtr left, Token* op, ExprPtr right) : left(left), op(op), right(right) {
        type = AstNodeType::BinaryExpression;
    }
    ExprPtr left;
    Token* op;
    ExprPtr right;
};

#endif //MATURSKI_2_BINARYEXPRESSION_H
