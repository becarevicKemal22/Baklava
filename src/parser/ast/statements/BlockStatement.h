//
// Created by kemal on 3/16/2024.
//

#ifndef MATURSKI_2_BLOCKSTATEMENT_H
#define MATURSKI_2_BLOCKSTATEMENT_H

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

#endif //MATURSKI_2_BLOCKSTATEMENT_H
