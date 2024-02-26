//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_PROGRAM_H
#define MATURSKI_2_PROGRAM_H

#include <vector>
#include "Statement.h"

/**
 * @brief Represents the entire program.
 *
 * Contains a vector of pointers to all statements in the program which are to be executed.
 */
class Program : public Statement {
public:
    Program() {
        type = AstNodeType::Program;
    }
    std::vector<Statement*> statements;
};

#endif //MATURSKI_2_PROGRAM_H
