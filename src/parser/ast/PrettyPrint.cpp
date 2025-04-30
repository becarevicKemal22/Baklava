//
// Created by kemal on 2/5/2024.
//

#include <iostream>

#include "PrettyPrint.h"
#include "Program.h"

#include "BinaryExpression.h"
#include "Statement.h"
#include "UnaryExpression.h"
#include "NullLiteralExpression.h"
#include "BooleanLiteralExpression.h"
#include "NumericLiteralExpression.h"
#include "StringLiteralExpression.h"
#include "GroupingExpression.h"
#include "ExpressionStatement.h"
#include "PrintStatement.h"
#include "VariableExpression.h"
#include "VarDeclarationStatement.h"
#include "BlockStatement.h"
#include "WhileStatement.h"
#include "AssignmentExpression.h"
#include "CallExpression.h"
#include "FunctionDeclarationStatement.h"
#include "IndexingExpression.h"
#include "ArrayLiteralExpression.h"
#include "IndexAssignmentExpression.h"
#include "IfStatement.h"
#include "ReturnStatement.h"
#include "LogicalExpression.h"
#include "ClassDeclarationStatement.h"

#define INDENTATION_PER_LEVEL 2

void printStatement(Statement *statement, int depth);

void printAST(std::unique_ptr<Program> &program) {
    std::wcout << L"Program" << std::endl;
    for (auto statement: program->statements) {
        printStatement(statement, 0);
    }
}

void indent(int depth) {
    for (int i = 0; i < depth * INDENTATION_PER_LEVEL; i++) {
        if (i % INDENTATION_PER_LEVEL == 0 && i != 0)
            std::wcout << L"|";
        else
            std::wcout << L" ";
    }
}

void printBinaryExpression(BinaryExpression *expression, int depth) {
    std::wcout << L"BinExpr( ";
    std::wcout << expression->op->value << " ";
    printStatement(expression->left, depth + 1);
    std::wcout << L", ";
    printStatement(expression->right, depth + 1);
    std::wcout << L") ";
}

void printUnaryExpression(UnaryExpression *expression, int depth) {
    std::wcout << L"UnaryExpr( ";
    std::wcout << expression->op->value;
    printStatement(expression->expr, depth + 1);
    std::wcout << L") ";
}

void printNullLiteralExpression(NullLiteralExpression *expression, int depth) {
    std::wcout << L"NullLit ";
}

void printBooleanLiteralExpression(BooleanLiteralExpression *expression, int depth) {
    std::wcout << L"BooleanLit(" << expression->value << L") ";
}

void printNumericLiteralExpression(NumericLiteralExpression *expression, int depth) {
    std::wcout << L"NumericLit(" << expression->value << L") ";
}

void printStringLiteralExpression(StringLiteralExpression *expression, int depth) {
    std::wcout << L"StringLit(" << expression->value << L") ";
}

void printVariableExpression(VariableExpression *expression, int depth) {
    std::wcout << L"VarExpr(" << expression->name->value << L") ";
}

void printCallExpression(CallExpression *expression, int depth) {
    std::wcout << L"CallExpr( ";
    printStatement(expression->callee, depth + 1);
    for (auto arg: expression->arguments) {
        printStatement(arg, depth + 1);
        std::wcout << L", ";
    }
    std::wcout << L" ) ";
}

void printIndexingExpression(IndexingExpression *expression, int depth) {
    std::wcout << L"IndexingExpr( ";
    printStatement(expression->left, depth + 1);
    printStatement(expression->index, depth + 1);
    std::wcout << L" ) ";
}

void printFunctionDeclarationStatement(FunctionDeclarationStatement *statement, int depth) {
    std::wcout << L"FunDeclStmt( ";
    std::wcout << static_cast<FunctionDeclarationStatement *>(statement)->name->value << L" ";
    std::wcout << L"[ ";
    auto params = static_cast<FunctionDeclarationStatement *>(statement)->parameters;
    auto defaultParams = static_cast<FunctionDeclarationStatement *>(statement)->defaultParameters;
    int size = params.size();
    int defaultSize = defaultParams.size();
    for (int i = 0; i < size; i++) {
        std::wcout << params[i]->value;
        if (i >= size - defaultSize) {
            std::wcout << L" = ";
            printStatement(defaultParams[i - (size - defaultSize)], depth + 1);
        }
        if (i != size - 1)
            std::wcout << L", ";
    }
    std::wcout << L" ] ) \n";
    for (auto stmt: static_cast<FunctionDeclarationStatement *>(statement)->body) {
        printStatement(stmt, depth + 1);
    }
}

void printClassDeclarationStatement(ClassDeclarationStatement *statement, int depth) {
    std::wcout << L"ClassDeclStmt( ";
    std::wcout << statement->name->value << L" ) \n";
    for (auto method: statement->methods) {
        indent(depth + 1);
        printFunctionDeclarationStatement(method, depth + 1);
    }
}

void printStatement(Statement *statement, int depth) {
    if (!dynamic_cast<Expression *>(statement)) {
        indent(depth);
    }

    AstNodeType type = statement->type;
    switch (type) {
        case AstNodeType::BinaryExpression:
            printBinaryExpression(static_cast<BinaryExpression *>(statement), depth);
            break;
        case AstNodeType::UnaryExpression:
            printUnaryExpression(static_cast<UnaryExpression *>(statement), depth);
            break;
        case AstNodeType::NullLiteralExpression:
            printNullLiteralExpression(static_cast<NullLiteralExpression *>(statement), depth);
            break;
        case AstNodeType::BooleanLiteralExpression:
            printBooleanLiteralExpression(static_cast<BooleanLiteralExpression *>(statement), depth);
            break;
        case AstNodeType::NumericLiteralExpression:
            printNumericLiteralExpression(static_cast<NumericLiteralExpression *>(statement), depth);
            break;
        case AstNodeType::StringLiteralExpression:
            printStringLiteralExpression(static_cast<StringLiteralExpression *>(statement), depth);
            break;
        case AstNodeType::GroupingExpression:
            std::wcout << L"GroupingExpr( ";
            printStatement(static_cast<GroupingExpression *>(statement)->expr, depth + 1);
            std::wcout << L" ) ";
            break;
        case AstNodeType::LogicalExpression:
            std::wcout << L"LogicalExpr( ";
            std::wcout << static_cast<LogicalExpression *>(statement)->op->value << L" ";
            printStatement(static_cast<LogicalExpression *>(statement)->left, depth + 1);
            std::wcout << L", ";
            printStatement(static_cast<LogicalExpression *>(statement)->right, depth + 1);
            std::wcout << L" ) ";
            break;
        case AstNodeType::ExpressionStatement:
            std::wcout << L"ExprStmt( ";
            printStatement(static_cast<ExpressionStatement *>(statement)->expr, depth + 1);
            std::wcout << L" ) \n";
            break;
        case AstNodeType::PrintStatement:
            std::wcout << L"PrintStmt( ";
            printStatement(static_cast<PrintStatement *>(statement)->expr, depth + 1);
            std::wcout << L" ) \n";
            break;
        case AstNodeType::AssignmentExpression:
            std::wcout << L"AssignmentExpr( ";
            std::wcout << static_cast<AssignmentExpression *>(statement)->name->value << L" ";
            printStatement(static_cast<AssignmentExpression *>(statement)->value, depth + 1);
            std::wcout << L" ) ";
            break;
        case AstNodeType::IndexAssignmentExpression:
            std::wcout << L"IndexAssignmentExpr( ";
            printStatement(static_cast<IndexAssignmentExpression *>(statement)->left, depth + 1);
            printStatement(static_cast<IndexAssignmentExpression *>(statement)->index, depth + 1);
            printStatement(static_cast<IndexAssignmentExpression *>(statement)->value, depth + 1);
            std::wcout << L" ) ";
            break;
        case AstNodeType::VarDeclarationStatement:
            if (static_cast<VarDeclarationStatement *>(statement)->isConst)
                std::wcout << L"ConstDeclStmt( ";
            else
                std::wcout << L"VarDeclStmt( ";
            std::wcout << static_cast<VarDeclarationStatement *>(statement)->name->value << L" ";
            if (static_cast<VarDeclarationStatement *>(statement)->initializer != nullptr) {
                printStatement(static_cast<VarDeclarationStatement *>(statement)->initializer, depth + 1);
            }
            std::wcout << L" ) \n";
            break;
        case AstNodeType::VariableExpression:
            printVariableExpression(static_cast<VariableExpression *>(statement), depth);
            break;
        case AstNodeType::BlockStatement:
            std::wcout << L"BlockStmt" << std::endl;
            for (auto stmt: static_cast<BlockStatement *>(statement)->statements) {
                printStatement(stmt, depth + 1);
            }
            break;
        case AstNodeType::WhileStatement:
            std::wcout << L"WhileStmt( ";
            printStatement(static_cast<WhileStatement *>(statement)->condition, depth + 1);
            std::wcout << L" ) \n";
            printStatement(static_cast<WhileStatement *>(statement)->body, depth + 1);
            break;
        case AstNodeType::IfStatement:
            std::wcout << L"IfStmt( ";
            printStatement(static_cast<IfStatement *>(statement)->condition, depth + 1);
            std::wcout << L" ) \n";
            printStatement(static_cast<IfStatement *>(statement)->thenBranch, depth + 1);
            if (static_cast<IfStatement *>(statement)->elseBranch != nullptr) {
                indent(depth);
                std::wcout << L"Else" << std::endl;
                printStatement(static_cast<IfStatement *>(statement)->elseBranch, depth + 1);
            }
            break;
        case AstNodeType::CallExpression:
            printCallExpression(static_cast<CallExpression *>(statement), depth);
            break;
        case AstNodeType::FunctionDeclarationStatement: {
            printFunctionDeclarationStatement(static_cast<FunctionDeclarationStatement *>(statement), depth);
            break;
        }
        case AstNodeType::ReturnStatement:
            std::wcout << L"ReturnStmt( ";
            if (static_cast<ReturnStatement *>(statement)->value != nullptr) {
                printStatement(static_cast<ReturnStatement *>(statement)->value, depth + 1);
            }
            std::wcout << L" ) \n";
            break;
        case AstNodeType::IndexingExpression:
            printIndexingExpression(static_cast<IndexingExpression *>(statement), depth);
            break;
        case AstNodeType::ArrayLiteralExpression:
            std::wcout << L"ArrLitExpr( ";
            for (auto element: static_cast<ArrayLiteralExpression *>(statement)->elements) {
                printStatement(element, depth + 1);
                std::wcout << L", ";
            }
            std::wcout << L" ) ";
            break;
        case AstNodeType::ClassDeclarationStatement: {
            printClassDeclarationStatement(static_cast<ClassDeclarationStatement *>(statement), depth);
            break;
        }
        default:
            std::wcout << L"Unknown statement type in PrintAST" << std::endl;
    }
}
