//
// Created by kemal on 3/16/2024.
//

#ifndef MATURSKI_2_WHILESTATEMENT_H
#define MATURSKI_2_WHILESTATEMENT_H

#include "Statement.h"

class WhileStatement : public Statement {
public:
    WhileStatement(ExprPtr condition, StmtPtr body) : condition(condition), body(body) {
        type = AstNodeType::WhileStatement;
    }
    ExprPtr condition;
    StmtPtr body;
};

#endif //MATURSKI_2_WHILESTATEMENT_H
