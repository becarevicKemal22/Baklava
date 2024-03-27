//
// Created by kemal on 3/20/2024.
//

#include "Resolver.h"

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

void Resolver::resolve(std::unique_ptr<Program>& program) {
    for (auto& statement : program->statements) {
        resolve(statement);
    }
}

void Resolver::resolve(const std::vector<Statement*>& statements) {
    for (auto& statement : statements) {
        resolve(statement);
    }
}

void Resolver::resolve(Statement* statement) {
    switch(statement->type) {
        case AstNodeType::VarDeclarationStatement:
            resolveVarDeclarationStatement(static_cast<VarDeclarationStatement*>(statement));
            break;
        case AstNodeType::FunctionDeclarationStatement:
            resolveFunctionDeclarationStatement(static_cast<FunctionDeclarationStatement*>(statement));
            break;
        case AstNodeType::ExpressionStatement:
            resolveExpressionStatement(static_cast<ExpressionStatement*>(statement));
            break;
        case AstNodeType::PrintStatement:
            resolvePrintStatement(static_cast<PrintStatement*>(statement));
            break;
        case AstNodeType::BlockStatement:
            resolveBlockStatement(static_cast<BlockStatement*>(statement));
            break;
        case AstNodeType::IfStatement:
            resolveIfStatement(static_cast<IfStatement*>(statement));
            break;
        case AstNodeType::WhileStatement:
            resolveWhileStatement(static_cast<WhileStatement*>(statement));
            break;
        case AstNodeType::ReturnStatement:
            resolveReturnStatement(static_cast<ReturnStatement*>(statement));
            break;
    }
}

void Resolver::resolve(Expression* expression) {
    switch(expression->type) {
        case AstNodeType::VariableExpression:
            resolveVariableExpression(static_cast<VariableExpression*>(expression));
            break;
        case AstNodeType::AssignmentExpression:
            resolveAssignmentExpression(static_cast<AssignmentExpression*>(expression));
            break;
        case AstNodeType::BinaryExpression:
            resolveBinaryExpression(static_cast<BinaryExpression*>(expression));
            break;
        case AstNodeType::CallExpression:
            resolveCallExpression(static_cast<CallExpression*>(expression));
            break;
        case AstNodeType::GroupingExpression:
            resolveGroupingExpression(static_cast<GroupingExpression*>(expression));
            break;
        case AstNodeType::LogicalExpression:
            resolveLogicalExpression(static_cast<LogicalExpression*>(expression));
            break;
        case AstNodeType::UnaryExpression:
            resolveUnaryExpression(static_cast<UnaryExpression*>(expression));
            break;
        case AstNodeType::NumericLiteralExpression:
            resolveNumericLiteralExpression(static_cast<NumericLiteralExpression*>(expression));
            break;
        case AstNodeType::BooleanLiteralExpression:
            resolveBooleanLiteralExpression(static_cast<BooleanLiteralExpression*>(expression));
            break;
        case AstNodeType::StringLiteralExpression:
            resolveStringLiteralExpression(static_cast<StringLiteralExpression*>(expression));
            break;
        case AstNodeType::NullLiteralExpression:
            resolveNullLiteralExpression(static_cast<NullLiteralExpression*>(expression));
            break;
    }
}

void Resolver::declare(Token* name) {
    if (scopes.empty()) return;

    auto &scope = scopes.top();
    if(scope.find(name->value) != scope.end()){
        throw VariableRedeclaration(name);
    }
    scope.insert({name->value, false});
}

void Resolver::define(Token* name) {
    if (scopes.empty()) return;

    auto &scope = scopes.top();
    scope[name->value] = true;
}

void Resolver::resolveLocal(const Expression* expr, const std::wstring& name) {
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

void Resolver::resolveFunction(const FunctionDeclarationStatement* function, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (auto &param : function->parameters) {
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

void Resolver::resolveVarDeclarationStatement(VarDeclarationStatement* statement) {
    declare(statement->name);
    if (statement->initializer != nullptr) {
        resolve(statement->initializer);
    }
    define(statement->name);
}

void Resolver::resolveVariableExpression(VariableExpression *expression) {
    if(!scopes.empty()){
        auto &scope = scopes.top();
        if(scope.find(expression->name->value) != scope.end() && !scope[expression->name->value]){
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

void Resolver::resolveExpressionStatement(ExpressionStatement *statement) {
    resolve(statement->expr);
}

void Resolver::resolvePrintStatement(PrintStatement *statement) {
    if(statement->expr != nullptr){
        resolve(statement->expr);
    }
}

void Resolver::resolveIfStatement(IfStatement *statement) {
    resolve(statement->condition);
    resolve(statement->thenBranch);
    if(statement->elseBranch != nullptr){
        resolve(statement->elseBranch);
    }
}

void Resolver::resolveWhileStatement(WhileStatement *statement) {
    resolve(statement->condition);
    resolve(statement->body);
}

void Resolver::resolveReturnStatement(ReturnStatement *statement) {
    if(currentFunction == FunctionType::NONE){
        throw InvalidReturnPosition(statement->keyword);
    }
    if(statement->value != nullptr){
        resolve(statement->value);
    }
}

void Resolver::resolveBinaryExpression(BinaryExpression *expression) {
    resolve(expression->left);
    resolve(expression->right);
}

void Resolver::resolveCallExpression(CallExpression *expression) {
    resolve(expression->callee);
    for(auto &arg : expression->arguments){
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

void Resolver::resolveNumericLiteralExpression(NumericLiteralExpression *expression) {}
void Resolver::resolveStringLiteralExpression(StringLiteralExpression *expression) {}
void Resolver::resolveBooleanLiteralExpression(BooleanLiteralExpression *expression) {}
void Resolver::resolveNullLiteralExpression(NullLiteralExpression *expression) {}
