//
// Created by kemal on 27-Feb-25.
//

#ifndef CLASSDECLARATION_H
#define CLASSDECLARATION_H

#include <vector>

#include "Statement.h"
#include "Token.h"

/**
 * @brief Represents a class declaration statement.
 */
class ClassDeclarationStatement : public Statement {
public:
    ClassDeclarationStatement(const TokenPtr name, const std::vector<FunctionDeclarationStatement*> &methods) : name(name), methods(methods) {
        type = AstNodeType::ClassDeclarationStatement;
    }
    TokenPtr name;
    std::vector<FunctionDeclarationStatement*> methods;
};

#endif //CLASSDECLARATION_H
