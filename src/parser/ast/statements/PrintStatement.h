//
// Created by kemal on 3/12/2024.
//

#ifndef MATURSKI_2_PRINTSTATEMENT_H
#define MATURSKI_2_PRINTSTATEMENT_H

#include "Statement.h"

class PrintStatement : public Statement {
public:
    PrintStatement(ExprPtr expr) : expr(expr) {
        type = AstNodeType::PrintStatement;
    }
    ExprPtr expr;
};

#endif //MATURSKI_2_PRINTSTATEMENT_H
