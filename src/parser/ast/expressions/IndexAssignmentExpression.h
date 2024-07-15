//
// Created by kemal on 6/7/2024.
//

#ifndef BAKLAVA_INDEXASSIGNMENTEXPRESSION_H
#define BAKLAVA_INDEXASSIGNMENTEXPRESSION_H

#include "Expression.h"
#include "Token.h"

/**
 * @brief Represents an assignment at an array index (e.g. a[1]=5;).
 */
class IndexAssignmentExpression : public Expression {
public:
    IndexAssignmentExpression(ExprPtr left, ExprPtr index, ExprPtr value) : left(left), index(index), value(value) {
        type = AstNodeType::IndexAssignmentExpression;
    }
    ExprPtr left;
    ExprPtr index;
    ExprPtr value;
};


#endif //BAKLAVA_INDEXASSIGNMENTEXPRESSION_H
