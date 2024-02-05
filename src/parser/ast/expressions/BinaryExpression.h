//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_BINARYEXPRESSION_H
#define MATURSKI_2_BINARYEXPRESSION_H

#include "Expression.h"
#include "Ast.h"
#include "Token.h"

class BinaryExpression : public Expression {
public:
    BinaryExpression(NodeRef left, Token* op, NodeRef right) : left(left), op(op), right(right) {
        type = AstNodeType::BinaryExpression;
    }
    NodeRef left;
    Token* op;
    NodeRef right;
};

#endif //MATURSKI_2_BINARYEXPRESSION_H
