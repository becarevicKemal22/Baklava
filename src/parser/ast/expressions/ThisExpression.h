//
// Created by kemal on 09-Feb-26.
//

#ifndef THISEXPRESSION_H
#define THISEXPRESSION_H

#include <string>

#include "Token.h"
#include "Expression.h"

/**
 * @brief Represents a 'this' expression.
 */
class ThisExpression : public Expression {
public:
    ThisExpression(Token* token) : token(token) {
        type = AstNodeType::ThisExpression;
        value = token->value;
    }
    Token* token;
    std::wstring value;
};

#endif //THISEXPRESSION_H
