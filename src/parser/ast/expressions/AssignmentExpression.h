//
// Created by kemal on 3/13/2024.
//

#ifndef MATURSKI_2_ASSIGNMENTEXPRESSION_H
#define MATURSKI_2_ASSIGNMENTEXPRESSION_H

#include "Expression.h"
#include "Token.h"

/**
 * @brief Represents an assignment expression.
 */
class AssignmentExpression : public Expression {
public:
    AssignmentExpression(TokenPtr name, ExprPtr value) : name(name), value(value) {
        type = AstNodeType::AssignmentExpression;
    }
    TokenPtr name;
    ExprPtr value;
};

#endif //MATURSKI_2_ASSIGNMENTEXPRESSION_H
