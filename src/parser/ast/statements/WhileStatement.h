//
// Created by kemal on 3/16/2024.
//

#ifndef BAKLAVA_WHILESTATEMENT_H
#define BAKLAVA_WHILESTATEMENT_H

#include "Statement.h"

class WhileStatement : public Statement {
public:
    WhileStatement(ExprPtr condition, StmtPtr body) : condition(condition), body(body) {
        type = AstNodeType::WhileStatement;
    }
    ExprPtr condition;
    StmtPtr body;
};

#endif //BAKLAVA_WHILESTATEMENT_H
