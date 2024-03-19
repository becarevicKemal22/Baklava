//
// Created by kemal on 3/19/2024.
//

#ifndef MATURSKI_2_RETURNSTATEMENT_H
#define MATURSKI_2_RETURNSTATEMENT_H

#include "Statement.h"
#include "Token.h"

class ReturnStatement : public Statement {
public:
    ReturnStatement(TokenPtr keyword, ExprPtr value) : keyword(keyword), value(value) {
        type = AstNodeType::ReturnStatement;
    }

    TokenPtr keyword;
    ExprPtr value;
};

#endif //MATURSKI_2_RETURNSTATEMENT_H
