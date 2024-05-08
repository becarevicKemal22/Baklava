//
// Created by kemal on 3/16/2024.
//

#ifndef BAKLAVA_BLOCKSTATEMENT_H
#define BAKLAVA_BLOCKSTATEMENT_H

#include <utility>
#include <vector>
#include "Statement.h"

class BlockStatement : public Statement {
public:
    explicit BlockStatement(std::vector<StmtPtr> statements) : statements(std::move(statements)) {
        type = AstNodeType::BlockStatement;
    }
    std::vector<StmtPtr> statements;
};

#endif //BAKLAVA_BLOCKSTATEMENT_H
