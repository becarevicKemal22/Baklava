//
// Created by kemal on 05-Jul-25.
//

#ifndef GETEXPRESSION_H
#define GETEXPRESSION_H

#include "Expression.h"
#include "Token.h"
#include <vector>

/**
 * @brief Represents a "get" (dot) expression for propery access on objects.
 */
class GetExpression : public Expression {
public:
    GetExpression(ExprPtr object, TokenPtr name) : object(object), name(name) {
        type = AstNodeType::GetExpression;
    }
    Expression* object;  // The object from which the property is being accessed
    TokenPtr name;       // The name of the property being accessed

};

#endif //GETEXPRESSION_H
