//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_NULLLITERALEXPRESSION_H
#define MATURSKI_2_NULLLITERALEXPRESSION_H

#include "Expression.h"
#include "Token.h"

class NullLiteralExpression : public Expression {
public:
    NullLiteralExpression(TokenPtr token) : token(token) {
        type = AstNodeType::NullLiteralExpression;
    }
    TokenPtr token;
};

#endif //MATURSKI_2_NULLLITERALEXPRESSION_H
