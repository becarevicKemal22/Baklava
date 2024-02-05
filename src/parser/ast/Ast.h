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
    UnaryExpression,
    NullLiteralExpression,
    BooleanLiteralExpression,
    NumericLiteralExpression,
    StringLiteralExpression,
};

typedef unsigned int NodeRef;

class Statement;
class Program;

class Expression;
class BinaryExpression;
class UnaryExpression;
class NullLiteralExpression;
class BooleanLiteralExpression;
class NumericLiteralExpression;
class StringLiteralExpression;

typedef Expression* ExprPtr;

#endif //MATURSKI_2_AST_H