//
// Created by kemal on 09-Jul-25.
//

#ifndef SETEXPRESSION_H
#define SETEXPRESSION_H

#include "Expression.h"
#include "Token.h"
#include <vector>

/**
 * @brief Represents a property assignment expression, where a property of an object is being set.
 */
class SetExpression : public Expression {
public:
    SetExpression(ExprPtr object, TokenPtr name, ExprPtr value) : object(object), name(name), value(value) {
        type = AstNodeType::SetExpression;
    }
    ExprPtr object;      // The object from which the property is being accessed
    TokenPtr name;       // The name of the property being accessed
    ExprPtr value;       // Value to assign

};

#endif //SETEXPRESSION_H
