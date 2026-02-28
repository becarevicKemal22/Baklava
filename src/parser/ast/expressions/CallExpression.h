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
    CallExpression(ExprPtr callee, TokenPtr paren, std::vector<ExprPtr> arguments, bool isNewPrefixed) : callee(callee),
        paren(paren), arguments(arguments), isNewPrefixed(isNewPrefixed) {
        type = AstNodeType::CallExpression;
    }

    ExprPtr callee;
    TokenPtr paren;
    std::vector<ExprPtr> arguments;
    const bool isNewPrefixed;
};

#endif //BAKLAVA_CALLEXPRESSION_H
