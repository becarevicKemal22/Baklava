//
// Created by kemal on 3/13/2024.
//

#ifndef MATURSKI_2_VARDECLARATIONSTATEMENT_H
#define MATURSKI_2_VARDECLARATIONSTATEMENT_H

#include "Statement.h"
#include "Token.h"

/**
 * @brief Represents a variable declaration statement.
 */
class VarDeclarationStatement : public Statement {
public:
    VarDeclarationStatement(TokenPtr name, ExprPtr initializer, bool isConst) : name(name), initializer(initializer), isConst(isConst) {
        type = AstNodeType::VarDeclarationStatement;
    }
    TokenPtr name;
    ExprPtr initializer;
    bool isConst;
};

#endif //MATURSKI_2_VARDECLARATIONSTATEMENT_H
