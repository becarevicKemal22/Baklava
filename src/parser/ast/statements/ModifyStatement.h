//
// Created by kemal on 25-Feb-25.
//

#ifndef MODIFYSTATEMENT_H
#define MODIFYSTATEMENT_H

#include "Statement.h"

enum ModificationType {
    INCREMENT,
    DECREMENT,
    MULTIPLY,
    DIVIDE
};

/**
 * @brief Represents a group of statements consisting of 'povecaj', 'smanji', 'pomnozi sa' and 'podijeli sa' statements.
 */
class ModifyStatement : public Statement {
public:
    ModifyStatement(const ExprPtr lvalue, const ExprPtr by, const TokenPtr keyword) : lvalue(lvalue), by(by), keyword(keyword) {
        type = AstNodeType::ModifyStatement;
        switch (keyword->type) {
            case TokenType::Povecaj:
                modificationType = INCREMENT;
                break;
            case TokenType::Smanji:
                modificationType = DECREMENT;
                break;
            case TokenType::Pomnozi:
                modificationType = MULTIPLY;
                break;
            case TokenType::Podijeli:
                modificationType = DIVIDE;
                break;
            default:
                throw "Wrong keyword type for ModifyStatement";
        }
    }

    ExprPtr lvalue;
    ExprPtr by;
    ModificationType modificationType;
    TokenPtr keyword; /**< The keyword /operation used to begin the statement. Used for error reporting. */
};

#endif //MODIFYSTATEMENT_H
