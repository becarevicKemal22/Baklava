//
// Created by kemal on 2/5/2024.
//

#ifndef BAKLAVA_NULLLITERALEXPRESSION_H
#define BAKLAVA_NULLLITERALEXPRESSION_H

#include "Expression.h"
#include "Token.h"

/**
 * @brief Represents a null literal expression.
 */
class NullLiteralExpression : public Expression {
public:
    NullLiteralExpression(TokenPtr token) : token(token) {
        type = AstNodeType::NullLiteralExpression;
    }
    TokenPtr token;
};

#endif //BAKLAVA_NULLLITERALEXPRESSION_H
