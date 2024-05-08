//
// Created by kemal on 3/16/2024.
//

#ifndef BAKLAVA_IFSTATEMENT_H
#define BAKLAVA_IFSTATEMENT_H

#include "Statement.h"

class IfStatement : public Statement {
public:
    IfStatement(ExprPtr condition, StmtPtr thenBranch, StmtPtr elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {
        type = AstNodeType::IfStatement;
    }
    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch;
};

#endif //BAKLAVA_IFSTATEMENT_H
