//
// Created by kemal on 3/12/2024.
//

#ifndef MATURSKI_2_GROUPINGEXPRESSION_H
#define MATURSKI_2_GROUPINGEXPRESSION_H

#include "Expression.h"
#include "Ast.h"
#include "Token.h"

/**
 * @brief Represents a parenthesized grouping expression.
 */
class GroupingExpression : public Expression {
public:
    GroupingExpression(ExprPtr expr) : expr(expr) {
        type = AstNodeType::GroupingExpression;
    }
    ExprPtr expr;
};

#endif //MATURSKI_2_GROUPINGEXPRESSION_H
