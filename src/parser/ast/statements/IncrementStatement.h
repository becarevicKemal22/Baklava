//
// Created by kemal on 25-Feb-25.
//

#ifndef INCREMENTSTATEMENT_H
#define INCREMENTSTATEMENT_H

#include "Statement.h"

/**
 * @brief Represents both an increment and decrement statement. The statement is of the form 'povecaj a;' or 'smanji a za 2;'. isDecrement is used to differentiate between increment and decrement, to avoid having two essentially identical AST nodes.
 */
class IncrementStatement : public Statement {
public:
    IncrementStatement(ExprPtr lvalue, ExprPtr by, TokenPtr keyword) : lvalue(lvalue), by(by), isDecrement(isDecrement), keyword(keyword) {
        type = AstNodeType::IncrementStatement;
        isDecrement = keyword->type == TokenType::Smanji;
    }

    ExprPtr lvalue;
    ExprPtr by;
    bool isDecrement; /**< True if the statement is a decrement statement, false if it is an increment statement. */
    TokenPtr keyword; /**< The keyword used to create the statement. Used for error reporting. Can either be 'povecaj' or 'smanji'. */
};

#endif //INCREMENTSTATEMENT_H
