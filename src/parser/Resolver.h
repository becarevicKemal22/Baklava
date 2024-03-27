//
// Created by kemal on 3/20/2024.
//

#ifndef MATURSKI_2_RESOLVER_H
#define MATURSKI_2_RESOLVER_H

#include <vector>
#include <memory>
#include <stack>
#include <unordered_map>
#include "Interpreter.h"
#include "Program.h"
#include "Statement.h"
#include "Expression.h"
#include "Ast.h"

typedef std::unordered_map<std::wstring, bool> Scope;
typedef std::stack<Scope> ScopeStack;

enum class FunctionType {
    NONE,
    FUNCTION,
};

class Resolver {
public:
    Resolver(Interpreter *interpreter) {
        this->interpreter = interpreter;
    }

    ~Resolver() = default;

    void resolve(std::unique_ptr<Program> &program);

    void resolve(const std::vector<Statement *> &statements);

    void resolve(Statement *statement);

    void resolve(Expression *expression);

private:
    ScopeStack scopes;
    Interpreter *interpreter;
    FunctionType currentFunction = FunctionType::NONE;

    void beginScope() {
        scopes.emplace();
    }

    void endScope() {
        scopes.pop();
    }

    void declare(Token *name);

    void define(Token *name);

    void resolveLocal(const Expression *expr, const std::wstring &name);

    void resolveFunction(const FunctionDeclarationStatement *function, FunctionType type);

    void resolveBlockStatement(BlockStatement *statement);

    void resolveVarDeclarationStatement(VarDeclarationStatement *statement);

    void resolveFunctionDeclarationStatement(FunctionDeclarationStatement *statement);

    void resolveExpressionStatement(ExpressionStatement *statement);

    void resolvePrintStatement(PrintStatement *statement);

    void resolveIfStatement(IfStatement *statement);

    void resolveWhileStatement(WhileStatement *statement);

    void resolveReturnStatement(ReturnStatement *statement);

    void resolveVariableExpression(VariableExpression *expression);

    void resolveAssignmentExpression(AssignmentExpression *expression);

    void resolveBinaryExpression(BinaryExpression *expression);

    void resolveCallExpression(CallExpression *expression);

    void resolveGroupingExpression(GroupingExpression *expression);

    void resolveLogicalExpression(LogicalExpression *expression);

    void resolveUnaryExpression(UnaryExpression *expression);

    void resolveNumericLiteralExpression(NumericLiteralExpression *expression);

    void resolveBooleanLiteralExpression(BooleanLiteralExpression *expression);

    void resolveStringLiteralExpression(StringLiteralExpression *expression);

    void resolveNullLiteralExpression(NullLiteralExpression *expression);

};


#endif //MATURSKI_2_RESOLVER_H
