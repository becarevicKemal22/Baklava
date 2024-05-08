//
// Created by kemal on 3/17/2024.
//

#ifndef BAKLAVA_FUNCTIONDECLARATIONSTATEMENT_H
#define BAKLAVA_FUNCTIONDECLARATIONSTATEMENT_H

#include "Statement.h"
#include "Token.h"
#include <vector>

class FunctionDeclarationStatement : public Statement {
public:
    FunctionDeclarationStatement(TokenPtr name, std::vector<TokenPtr> parameters, std::vector<StmtPtr> body)
            : name(name), parameters(parameters), body(body) {
        type = AstNodeType::FunctionDeclarationStatement;
    }

    TokenPtr name;
    std::vector<TokenPtr> parameters;
    std::vector<StmtPtr> body;
};

#endif //BAKLAVA_FUNCTIONDECLARATIONSTATEMENT_H
