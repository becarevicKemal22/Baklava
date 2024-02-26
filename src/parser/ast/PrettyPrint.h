//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_PRETTYPRINT_H
#define MATURSKI_2_PRETTYPRINT_H

#include <memory>
#include "Ast.h"

/**
 * @brief Pretty prints entire AST to standard output.
 * @param program unique pointer to the program to be printed.
 */
void printAST(std::unique_ptr<Program> &program);

#endif //MATURSKI_2_PRETTYPRINT_H
