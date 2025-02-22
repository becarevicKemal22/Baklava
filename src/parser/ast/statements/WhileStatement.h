//
// Created by kemal on 3/16/2024.
//

#ifndef BAKLAVA_WHILESTATEMENT_H
#define BAKLAVA_WHILESTATEMENT_H

#include "Statement.h"

class WhileStatement : public Statement {
public:
    WhileStatement(ExprPtr condition, StmtPtr body) : condition(condition), body(body) {
        type = AstNodeType::WhileStatement;
    }
    ExprPtr condition;
    StmtPtr body;
    bool isForLoop = false; /**< Is set to true when the while loop is created while parsing a for loop. Symbolizes whether the while loop servers as a for loop or normal while loop. */
    ExprPtr forIncrement = nullptr; /**< Necessary for for loops. Represents the increment expression of the for loop. It's probably better to keep this here instead of doing static casts and searching around the body in order to find the increment expression. */
};

#endif //BAKLAVA_WHILESTATEMENT_H
