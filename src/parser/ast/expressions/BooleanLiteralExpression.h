//
// Created by kemal on 2/5/2024.
//

#ifndef BAKLAVA_BOOLEANLITERALEXPRESSION_H
#define BAKLAVA_BOOLEANLITERALEXPRESSION_H

#include "Expression.h"
#include "Token.h"

/**
 * @brief Represents a boolean literal expression.
 */
class BooleanLiteralExpression : public Expression {
public:
    BooleanLiteralExpression(TokenPtr token) : token(token) {
        type = AstNodeType::BooleanLiteralExpression;
        value = token->type == TokenType::True;
    }
    TokenPtr token;
    bool value;
};

#endif //BAKLAVA_BOOLEANLITERALEXPRESSION_H
