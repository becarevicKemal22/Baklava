//
// Created by kemal on 3/16/2024.
//

#ifndef MATURSKI_2_IFSTATEMENT_H
#define MATURSKI_2_IFSTATEMENT_H

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

#endif //MATURSKI_2_IFSTATEMENT_H
