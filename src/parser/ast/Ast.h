//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_AST_H
#define MATURSKI_2_AST_H

#include <cstdint>

enum class AstNodeType : uint8_t {
    // Statements
    Program,

    // Expressions
    BinaryExpression,
    Unary,
    Primary,
};

typedef unsigned int NodeRef;

class Statement;
class Program;

#endif //MATURSKI_2_AST_H
