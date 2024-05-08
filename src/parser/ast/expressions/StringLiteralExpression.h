//
// Created by kemal on 2/5/2024.
//

#ifndef BAKLAVA_STRINGLITERALEXPRESSION_H
#define BAKLAVA_STRINGLITERALEXPRESSION_H

#include <string>

#include "Token.h"
#include "Expression.h"

/**
 * @brief Represents a string literal expression.
 */
class StringLiteralExpression : public Expression {
public:
    StringLiteralExpression(Token* token) : token(token) {
        type = AstNodeType::StringLiteralExpression;
        value = token->value;
    }
    Token* token;
    std::wstring value;
};

#endif //BAKLAVA_STRINGLITERALEXPRESSION_H
