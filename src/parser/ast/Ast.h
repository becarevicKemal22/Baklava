//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_AST_H
#define MATURSKI_2_AST_H

#include <cstdint>

/**
 * @brief Enum class representing the type of the AST node.
 *
 * Used to determine the type of a node at runtime without doing dynamic casting.
 */
enum class AstNodeType : uint8_t {
    // Statements
    Program,

    // Expressions
    BinaryExpression,
    LogicalExpression,
    UnaryExpression,
    NullLiteralExpression,
    BooleanLiteralExpression,
    NumericLiteralExpression,
    StringLiteralExpression,
    GroupingExpression,
};

class Statement;
class Program;

class Expression;
class BinaryExpression;
class LogicalExpression;
class UnaryExpression;
class NullLiteralExpression;
class BooleanLiteralExpression;
class NumericLiteralExpression;
class StringLiteralExpression;
class GroupingExpression;

typedef Statement* StmtPtr;
typedef Expression* ExprPtr;

#endif //MATURSKI_2_AST_H
