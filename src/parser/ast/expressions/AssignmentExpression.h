//
// Created by kemal on 3/13/2024.
//

#ifndef BAKLAVA_ASSIGNMENTEXPRESSION_H
#define BAKLAVA_ASSIGNMENTEXPRESSION_H

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

#endif //BAKLAVA_ASSIGNMENTEXPRESSION_H
