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
    ExpressionStatement,
    PrintStatement,
    VarDeclarationStatement,
    BlockStatement,

    // Expressions
    AssignmentExpression,
    BinaryExpression,
    LogicalExpression,
    UnaryExpression,
    NullLiteralExpression,
    BooleanLiteralExpression,
    NumericLiteralExpression,
    StringLiteralExpression,
    GroupingExpression,
    VariableExpression,
};

class Statement;
class Program;
class ExpressionStatement;
class PrintStatement;
class VarDeclarationStatement;
class BlockStatement;

class Expression;
class BinaryExpression;
class LogicalExpression;
class UnaryExpression;
class NullLiteralExpression;
class BooleanLiteralExpression;
class NumericLiteralExpression;
class StringLiteralExpression;
class GroupingExpression;
class VariableExpression;

typedef Statement* StmtPtr;
typedef Expression* ExprPtr;

#endif //MATURSKI_2_AST_H
