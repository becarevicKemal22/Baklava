//
// Created by kemal on 3/17/2024.
//

#ifndef BAKLAVA_CALLEXPRESSION_H
#define BAKLAVA_CALLEXPRESSION_H

#include "Expression.h"
#include "Token.h"
#include <vector>

class CallExpression : public Expression {
public:
    CallExpression(ExprPtr callee, TokenPtr paren, std::vector<ExprPtr> arguments) : callee(callee), paren(paren), arguments(arguments) {
        type = AstNodeType::CallExpression;
    }
    ExprPtr callee;
    TokenPtr paren;
    std::vector<ExprPtr> arguments;
};

#endif //BAKLAVA_CALLEXPRESSION_H
