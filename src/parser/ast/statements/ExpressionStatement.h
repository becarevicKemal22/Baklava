//
// Created by kemal on 3/12/2024.
//

#ifndef MATURSKI_2_EXPRESSIONSTATEMENT_H
#define MATURSKI_2_EXPRESSIONSTATEMENT_H

#include "Statement.h"

class ExpressionStatement : public Statement {
public:
    ExpressionStatement(ExprPtr expr) : expr(expr) {
        type = AstNodeType::ExpressionStatement;
    }
    ExprPtr expr;
};

#endif //MATURSKI_2_EXPRESSIONSTATEMENT_H
