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

void printStatement(Statement *statement, int depth);

void printAST(std::unique_ptr<Program> &program) {
    std::wcout << L"Program" << std::endl;
    for (auto statement: program->statements) {
        printStatement(statement, 1);
    }
}

void printBinaryExpression(BinaryExpression *expression, int depth) {
    std::wcout << L"BinExpr ( ";
    std::wcout << expression->op->value << " ";
    printStatement(expression->left, depth + 1);
    std::wcout << L", ";
    printStatement(expression->right, depth + 1);
    std::wcout << L" ) ";
}

void printUnaryExpression(UnaryExpression *expression, int depth) {
    std::wcout << L"UnaryExpr ( ";
    std::wcout << expression->op->value;
    printStatement(expression->expr, depth + 1);
    std::wcout << L" ) ";
}

void printNullLiteralExpression(NullLiteralExpression *expression, int depth) {
    std::wcout << L"NullLiteralExpr ";
}

void printBooleanLiteralExpression(BooleanLiteralExpression *expression, int depth) {
    std::wcout << L"BooleanLiteralExpr ( " << expression->value << L" ) ";
}

void printNumericLiteralExpression(NumericLiteralExpression *expression, int depth) {
    std::wcout << L"NumericLiteralExpr ( " << expression->value << L" ) ";
}

void printStringLiteralExpression(StringLiteralExpression *expression, int depth) {
    std::wcout << L"StringLiteralExpr ( " << expression->value << L" ) ";
}

void printVariableExpression(VariableExpression *expression, int depth) {
    std::wcout << L"VariableExpr ( " << expression->name->value << L" ) ";
}

void printCallExpression(CallExpression *expression, int depth) {
    std::wcout << L"CallExpr ( ";
    printStatement(expression->callee, depth + 1);
    for (auto arg: expression->arguments) {
        printStatement(arg, depth + 1);
    }
    std::wcout << L" ) ";
}

void printIndexingExpression(IndexingExpression *expression, int depth) {
    std::wcout << L"IndexingExpr(";
    printStatement(expression->left, depth + 1);
    printStatement(expression->index, depth + 1);
    std::wcout << L" ) ";
}

void printStatement(Statement *statement, int depth) {
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
            std::wcout << L"GroupingExpr ( ";
            printStatement(static_cast<GroupingExpression *>(statement)->expr, depth + 1);
            std::wcout << L" ) \n";
            break;
        case AstNodeType::ExpressionStatement:
            std::wcout << L"ExpressionStatement ( ";
            printStatement(static_cast<ExpressionStatement *>(statement)->expr, depth + 1);
            std::wcout << L" ) \n";
            break;
        case AstNodeType::PrintStatement:
            std::wcout << L"PrintStatement ( ";
            printStatement(static_cast<PrintStatement *>(statement)->expr, depth + 1);
            std::wcout << L" ) \n";
            break;
        case AstNodeType::AssignmentExpression:
            std::wcout << L"AssignmentExpression ( ";
            std::wcout << static_cast<AssignmentExpression *>(statement)->name->value << L" ";
            printStatement(static_cast<AssignmentExpression *>(statement)->value, depth + 1);
            std::wcout << L" ) \n";
            break;
        case AstNodeType::IndexAssignmentExpression:
            std::wcout << L"IndexAssignmentExpression ( ";
            printStatement(static_cast<IndexAssignmentExpression *>(statement)->left, depth + 1);
            printStatement(static_cast<IndexAssignmentExpression *>(statement)->index, depth + 1);
            printStatement(static_cast<IndexAssignmentExpression *>(statement)->value, depth + 1);
            std::wcout << L" ) \n";
            break;
        case AstNodeType::VarDeclarationStatement:
            if (static_cast<VarDeclarationStatement *>(statement)->isConst)
                std::wcout << L"ConstDeclarationStatement ( ";
            else
                std::wcout << L"VarDeclarationStatement ( ";
            std::wcout << static_cast<VarDeclarationStatement *>(statement)->name->value << L" ";
            printStatement(static_cast<VarDeclarationStatement *>(statement)->initializer, depth + 1);
            std::wcout << L" ) \n";
            break;
        case AstNodeType::VariableExpression:
            printVariableExpression(static_cast<VariableExpression *>(statement), depth);
            break;
        case AstNodeType::BlockStatement:
            std::wcout << L"BlockStatement" << std::endl;
            for (auto stmt: static_cast<BlockStatement *>(statement)->statements) {
                printStatement(stmt, depth + 1);
            }
            break;
        case AstNodeType::WhileStatement:
            std::wcout << L"WhileStatement ( ";
            printStatement(static_cast<WhileStatement *>(statement)->condition, depth + 1);
            std::wcout << L" ) \n";
            printStatement(static_cast<WhileStatement *>(statement)->body, depth + 1);
            break;
        case AstNodeType::CallExpression:
            printCallExpression(static_cast<CallExpression *>(statement), depth);
            break;
        case AstNodeType::FunctionDeclarationStatement:
            std::wcout << L"FunctionDeclarationStatement ( ";
            std::wcout << static_cast<FunctionDeclarationStatement *>(statement)->name->value << L" ";
            std::wcout << L" ) \n";
            break;
        case AstNodeType::IndexingExpression:
            printIndexingExpression(static_cast<IndexingExpression *>(statement), depth);
            break;
        case AstNodeType::ArrayLiteralExpression:
            std::wcout << L"ArrayLiteralExpression ( ";
            for (auto element: static_cast<ArrayLiteralExpression *>(statement)->elements) {
                printStatement(element, depth + 1);
            }
            std::wcout << L" ) \n";
            break;
        default:
            std::wcout << L"Unknown statement type in PrintAST" << std::endl;
    }
}