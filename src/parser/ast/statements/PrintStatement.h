//
// Created by kemal on 3/12/2024.
//

#ifndef BAKLAVA_PRINTSTATEMENT_H
#define BAKLAVA_PRINTSTATEMENT_H

#include "Statement.h"

class PrintStatement : public Statement {
public:
    PrintStatement(ExprPtr expr) : expr(expr) {
        type = AstNodeType::PrintStatement;
    }
    ExprPtr expr;
};

#endif //BAKLAVA_PRINTSTATEMENT_H
