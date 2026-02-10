//
// Created by kemal on 3/20/2024.
//

#include "Resolver.h"

#include <cmath>

#include "VarDeclarationStatement.h"
#include "VariableExpression.h"
#include "AssignmentExpression.h"
#include "FunctionDeclarationStatement.h"
#include "ExpressionStatement.h"
#include "PrintStatement.h"
#include "BlockStatement.h"
#include "IfStatement.h"
#include "WhileStatement.h"
#include "ReturnStatement.h"

#include "BinaryExpression.h"
#include "CallExpression.h"
#include "GroupingExpression.h"
#include "LogicalExpression.h"
#include "UnaryExpression.h"
#include "NumericLiteralExpression.h"
#include "BooleanLiteralExpression.h"
#include "StringLiteralExpression.h"
#include "NullLiteralExpression.h"
#include "InvalidReturnPosition.h"
#include "IndexAssignmentExpression.h"
#include "IndexingExpression.h"
#include "ArrayLiteralExpression.h"
#include "ClassDeclarationStatement.h"
#include "ModifyStatement.h"
#include "GetExpression.h"
#include "SetExpression.h"
#include "ThisExpression.h"

void Resolver::resolve(std::unique_ptr<Program> &program) {
    for (auto &statement: program->statements) {
        resolve(statement);
    }
}

void Resolver::resolve(const std::vector<Statement *> &statements) {
    for (auto &statement: statements) {
        resolve(statement);
    }
}

void Resolver::resolve(Statement *statement) {
    switch (statement->type) {
        case AstNodeType::VarDeclarationStatement:
            resolveVarDeclarationStatement(static_cast<VarDeclarationStatement *>(statement));
            break;
        case AstNodeType::FunctionDeclarationStatement:
            resolveFunctionDeclarationStatement(static_cast<FunctionDeclarationStatement *>(statement));
            break;
        case AstNodeType::ExpressionStatement:
            resolveExpressionStatement(static_cast<ExpressionStatement *>(statement));
            break;
        case AstNodeType::PrintStatement:
            resolvePrintStatement(static_cast<PrintStatement *>(statement));
            break;
        case AstNodeType::BlockStatement:
            resolveBlockStatement(static_cast<BlockStatement *>(statement));
            break;
        case AstNodeType::IfStatement:
            resolveIfStatement(static_cast<IfStatement *>(statement));
            break;
        case AstNodeType::WhileStatement:
            resolveWhileStatement(static_cast<WhileStatement *>(statement));
            break;
        case AstNodeType::ReturnStatement:
            resolveReturnStatement(static_cast<ReturnStatement *>(statement));
            break;
        case AstNodeType::ModifyStatement:
            resolveModifyStatement(static_cast<ModifyStatement *>(statement));
            break;
        case AstNodeType::ClassDeclarationStatement:
            resolveClassDeclarationStatement(static_cast<ClassDeclarationStatement *>(statement));
            break;
        default:
            std::wcout << L"Unknown statement type in resolver." << std::endl;
    }
}

void Resolver::resolve(Expression *expression) {
    switch (expression->type) {
        case AstNodeType::VariableExpression:
            resolveVariableExpression(static_cast<VariableExpression *>(expression));
            break;
        case AstNodeType::AssignmentExpression:
            resolveAssignmentExpression(static_cast<AssignmentExpression *>(expression));
            break;
        case AstNodeType::BinaryExpression:
            resolveBinaryExpression(static_cast<BinaryExpression *>(expression));
            break;
        case AstNodeType::CallExpression:
            resolveCallExpression(static_cast<CallExpression *>(expression));
            break;
        case AstNodeType::GroupingExpression:
            resolveGroupingExpression(static_cast<GroupingExpression *>(expression));
            break;
        case AstNodeType::LogicalExpression:
            resolveLogicalExpression(static_cast<LogicalExpression *>(expression));
            break;
        case AstNodeType::UnaryExpression:
            resolveUnaryExpression(static_cast<UnaryExpression *>(expression));
            break;
        case AstNodeType::NumericLiteralExpression:
            resolveNumericLiteralExpression(static_cast<NumericLiteralExpression *>(expression));
            break;
        case AstNodeType::BooleanLiteralExpression:
            resolveBooleanLiteralExpression(static_cast<BooleanLiteralExpression *>(expression));
            break;
        case AstNodeType::StringLiteralExpression:
            resolveStringLiteralExpression(static_cast<StringLiteralExpression *>(expression));
            break;
        case AstNodeType::NullLiteralExpression:
            resolveNullLiteralExpression(static_cast<NullLiteralExpression *>(expression));
            break;
        case AstNodeType::IndexingExpression:
            resolveIndexingExpression(static_cast<IndexingExpression *>(expression));
            break;
        case AstNodeType::ArrayLiteralExpression:
            resolveArrayLiteralExpression(static_cast<ArrayLiteralExpression *>(expression));
            break;
        case AstNodeType::IndexAssignmentExpression:
            resolveIndexAssignmentExpression(static_cast<IndexAssignmentExpression *>(expression));
            break;
        case AstNodeType::GetExpression:
            resolveGetExpression(static_cast<GetExpression *>(expression));
            break;
        case AstNodeType::SetExpression:
            resolveSetExpression(static_cast<SetExpression *>(expression));
            break;
        case AstNodeType::ThisExpression:
            resolveThisExpression(static_cast<ThisExpression *>(expression));
            break;
        default:
            std::wcout << L"Unknown expression type in resolver." << std::endl;
    }
}

void Resolver::declare(Token *name) {
    if (scopes.empty()) return;

    auto &scope = scopes.top();
    if (scope.find(name->value) != scope.end()) {
        throw VariableRedeclaration(name);
    }
    scope.insert({name->value, false});
}

void Resolver::define(Token *name) {
    if (scopes.empty()) return;

    auto &scope = scopes.top();
    scope[name->value] = true;
}

void Resolver::resolveLocal(const Expression *expr, const std::wstring &name) {
    ScopeStack scopesCopy(scopes);
    while (!scopesCopy.empty()) {
        auto &scope = scopesCopy.top();
        if (scope.find(name) != scope.end()) {
            interpreter->resolve(expr, scopes.size() - scopesCopy.size());
            return;
        }
        scopesCopy.pop();
    }
}

void Resolver::resolveFunction(const FunctionDeclarationStatement *function, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (auto &param: function->parameters) {
        declare(param);
        define(param);
    }
    resolve(function->body);
    endScope();
    currentFunction = enclosingFunction;
}

void Resolver::resolveBlockStatement(BlockStatement *statement) {
    beginScope();
    resolve(statement->statements);
    endScope();
}

void Resolver::resolveVarDeclarationStatement(VarDeclarationStatement *statement) {
    declare(statement->name);
    if (statement->initializer != nullptr) {
        resolve(statement->initializer);
    }
    define(statement->name);
}

void Resolver::resolveVariableExpression(VariableExpression *expression) {
    if (!scopes.empty()) {
        auto &scope = scopes.top();
        if (scope.find(expression->name->value) != scope.end() && !scope[expression->name->value]) {
            throw SelfReferencingInitializer(expression->name);
        }
    }
    resolveLocal(expression, expression->name->value);
}

void Resolver::resolveAssignmentExpression(AssignmentExpression *expression) {
    resolve(expression->value);
    resolveLocal(expression, expression->name->value);
}

void Resolver::resolveFunctionDeclarationStatement(FunctionDeclarationStatement *statement) {
    declare(statement->name);
    define(statement->name);
    resolveFunction(statement, FunctionType::FUNCTION);
}

void Resolver::resolveClassDeclarationStatement(ClassDeclarationStatement *statement) {
    ClassType enclosingClassType = currentClass;
    currentClass = ClassType::CLASS;

    declare(statement->name);
    define(statement->name);

    beginScope();
    scopes.top().emplace(L"ovo", true);
    scopes.top().emplace(L"ovaj", true);
    scopes.top().emplace(L"ova", true);
    scopes.top().emplace(L"ovi", true);

    for (auto method : statement->methods) {
        resolveFunction(method, FunctionType::METHOD);
    }

    endScope();
    currentClass = enclosingClassType;
}


void Resolver::resolveExpressionStatement(ExpressionStatement *statement) {
    resolve(statement->expr);
}

void Resolver::resolvePrintStatement(PrintStatement *statement) {
    if (statement->expr != nullptr) {
        resolve(statement->expr);
    }
}

void Resolver::resolveIfStatement(IfStatement *statement) {
    resolve(statement->condition);
    resolve(statement->thenBranch);
    if (statement->elseBranch != nullptr) {
        resolve(statement->elseBranch);
    }
}

void Resolver::resolveWhileStatement(WhileStatement *statement) {
    // Ovo je ovdje bilo dok sam mislio da ce moci da se za step koriste razne vrste izraza, ali je sad svakako ograniceno
    // na brojcane literale. Ovo svakako nije ni radilo kada sam htio da budu svi izrazi podrzani, tako da ne pomislim
    // da je ovo nesto sto se samo moze ukljuciti i radit ce.
//    if(statement->isForLoop){
//        if(statement->forIncrement != nullptr){
//            resolve(statement->forIncrement);
//        }
//    }
    resolve(statement->condition);
    resolve(statement->body);
}

void Resolver::resolveReturnStatement(ReturnStatement *statement) {
    if (currentFunction == FunctionType::NONE) {
        throw InvalidReturnPosition(statement->keyword);
    }
    if (statement->value != nullptr) {
        resolve(statement->value);
    }
}

void Resolver::resolveModifyStatement(ModifyStatement *statement) {
    resolve(statement->lvalue);
    resolve(statement->by);
}

void Resolver::resolveBinaryExpression(BinaryExpression *expression) {
    resolve(expression->left);
    resolve(expression->right);
}

void Resolver::resolveCallExpression(CallExpression *expression) {
    resolve(expression->callee);
    for (auto &arg: expression->arguments) {
        resolve(arg);
    }
}

void Resolver::resolveGroupingExpression(GroupingExpression *expression) {
    resolve(expression->expr);
}

void Resolver::resolveLogicalExpression(LogicalExpression *expression) {
    resolve(expression->left);
    resolve(expression->right);
}

void Resolver::resolveUnaryExpression(UnaryExpression *expression) {
    resolve(expression->expr);
}

void Resolver::resolveIndexingExpression(IndexingExpression *expression) {
    resolve(expression->left);
    resolve(expression->index);
}

void Resolver::resolveIndexAssignmentExpression(IndexAssignmentExpression *expression) {
    resolve(expression->left);
    resolve(expression->index);
    resolve(expression->value);
}

void Resolver::resolveArrayLiteralExpression(ArrayLiteralExpression *expression) {
    for (auto &element: expression->elements) {
        resolve(element);
    }
}

void Resolver::resolveGetExpression(GetExpression *expression) {
    resolve(expression->object);
}

void Resolver::resolveSetExpression(SetExpression *expression) {
    resolve(expression->value);
    resolve(expression->object);
}

void Resolver::resolveThisExpression(ThisExpression *expression) {
    if (currentClass == ClassType::NONE) {
        throw InvalidThisPosition(expression->token);
    }

    resolveLocal(expression, expression->value);
}


void Resolver::resolveNumericLiteralExpression(NumericLiteralExpression *expression) {}

void Resolver::resolveStringLiteralExpression(StringLiteralExpression *expression) {}

void Resolver::resolveBooleanLiteralExpression(BooleanLiteralExpression *expression) {}

void Resolver::resolveNullLiteralExpression(NullLiteralExpression *expression) {}
