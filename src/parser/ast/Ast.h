//
// Created by kemal on 2/5/2024.
//

#ifndef BAKLAVA_AST_H
#define BAKLAVA_AST_H

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
    IfStatement,
    WhileStatement,
    FunctionDeclarationStatement,
    ReturnStatement,

    // Expressions
    AssignmentExpression,
    IndexAssignmentExpression,
    BinaryExpression,
    LogicalExpression,
    UnaryExpression,
    NullLiteralExpression,
    BooleanLiteralExpression,
    NumericLiteralExpression,
    StringLiteralExpression,
    GroupingExpression,
    VariableExpression,
    CallExpression,
    IndexingExpression,
    ArrayLiteralExpression,
};

class Statement;
class Program;
class ExpressionStatement;
class PrintStatement;
class VarDeclarationStatement;
class BlockStatement;
class IfStatement;
class WhileStatement;
class FunctionDeclarationStatement;
class ReturnStatement;

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
class CallExpression;
class IndexingExpression;
class ArrayLiteralExpression;
class IndexAssignmentExpression;

typedef Statement* StmtPtr;
typedef Expression* ExprPtr;

#endif //BAKLAVA_AST_H
