//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_STRINGLITERALEXPRESSION_H
#define MATURSKI_2_STRINGLITERALEXPRESSION_H

#include <string>

#include "Token.h"
#include "Expression.h"

class StringLiteralExpression : public Expression {
public:
    StringLiteralExpression(Token* token) : token(token) {
        type = AstNodeType::StringLiteralExpression;
        value = token->value;
    }
    Token* token;
    std::wstring value;
};

#endif //MATURSKI_2_STRINGLITERALEXPRESSION_H
