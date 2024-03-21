//
// Created by kemal on 2/6/2024.
//

#include "Interpreter.h"
#include "Program.h"
#include "Expression.h"
#include "BinaryExpression.h"
#include "UnaryExpression.h"
#include "NumericLiteralExpression.h"
#include "BooleanLiteralExpression.h"
#include "NullLiteralExpression.h"
#include "StringLiteralExpression.h"
#include "RuntimeValue.h"
#include "LogicalExpression.h"
#include "ExpressionStatement.h"
#include "PrintStatement.h"
#include "VarDeclarationStatement.h"
#include "VariableExpression.h"
#include "BlockStatement.h"
#include "IfStatement.h"
#include "WhileStatement.h"
#include "CallExpression.h"

#include "WrongTypeError.h"
#include "WrongBinaryOperandTypes.h"
#include "GroupingExpression.h"
#include "FunctionDeclarationStatement.h"
#include "InvalidCall.h"
#include "InvalidArgumentCount.h"
#include "RuntimeValue.h"
#include "ReturnStatement.h"
#include "Return.h"
#include "Resolver.h"

#include <iostream>
#include <cmath>

void Interpreter::interpret(Program *program) {
    for (auto stmt: program->statements) {
        try {
            execute(stmt);
        } catch (RuntimeError &e) {
            if (errorPrinter != nullptr) errorPrinter->printRuntimeError(&e);
            hadError = true;
            handledError = reallocateError(&e);
            return;
        }
    }
}

void Interpreter::execute(Statement *stmt) {
#ifdef DEBUG_TRACK_EXECUTION
    executedStatements.push_back(stmt);
#endif
    switch (stmt->type) {
        case AstNodeType::ExpressionStatement:
            executeExpressionStatement(static_cast<ExpressionStatement *>(stmt));
            return;
        case AstNodeType::PrintStatement:
            executePrintStatement(static_cast<PrintStatement *>(stmt));
            return;
        case AstNodeType::VarDeclarationStatement:
            executeVarDeclarationStatement(static_cast<VarDeclarationStatement *>(stmt));
            return;
        case AstNodeType::BlockStatement:
            executeBlockStatement(static_cast<BlockStatement *>(stmt));
            return;
        case AstNodeType::IfStatement:
            executeIfStatement(static_cast<IfStatement *>(stmt));
            return;
        case AstNodeType::WhileStatement:
            executeWhileStatement(static_cast<WhileStatement *>(stmt));
            return;
        case AstNodeType::FunctionDeclarationStatement:
            executeFunctionDeclarationStatement(static_cast<FunctionDeclarationStatement *>(stmt));
            return;
        case AstNodeType::ReturnStatement:
            executeReturnStatement(static_cast<ReturnStatement *>(stmt));
            return;
        default:
            throw std::runtime_error("Unknown statement type");
    }
}

void Interpreter::executeExpressionStatement(ExpressionStatement *stmt) {
    evaluate(stmt->expr);
}

void Interpreter::executePrintStatement(PrintStatement *stmt) {
    RuntimeValue value = evaluate(stmt->expr);
#ifdef DEBUG_TRACK_PRINTING
    printHistory.push_back(value);
#endif
    switch (value.type) {
        case ValueType::Number:
            std::wcout << value.as.number << std::endl;
            return;
        case ValueType::Boolean:
            std::wcout << (value.as.boolean ? L"tačno" : L"netačno") << std::endl;
            return;
        case ValueType::Null:
            std::wcout << L"null" << std::endl;
            return;
        case ValueType::Object:
            if (IS_STRING_OBJ(value)) {
                std::wcout << GET_STRING_OBJ_VALUE(value) << std::endl;
                return;
            }
            throw "PRINT NOT YET IMPLEMENTED FOR NON STRING OBJECTS!";
        default:
            throw "UNKNOWN TYPE TO PRINT";
    }
}

void Interpreter::executeVarDeclarationStatement(VarDeclarationStatement *stmt) {
    RuntimeValue value;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer);
    } else {
        value = {ValueType::Null};
    }

    environment->define(stmt->name, value, stmt->isConst);
}

void Interpreter::executeBlockStatement(BlockStatement *stmt) {
    executeBlock(stmt->statements, new Environment(environment));
}

void Interpreter::executeBlock(const std::vector<Statement*>& statements, Environment* environment){
    Environment* previous = this->environment;
    this->environment = environment;
    try{
        for(auto s: statements){
            execute(s);
        }
    } catch (Return& e){
        this->environment = previous;
        delete environment;
        throw;
    }
    this->environment = previous;
    delete environment;
}

void Interpreter::executeIfStatement(IfStatement *stmt) {
    if (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->thenBranch);
    } else if (stmt->elseBranch != nullptr) {
        execute(stmt->elseBranch);
    }
}

void Interpreter::executeWhileStatement(WhileStatement *stmt) {
    while (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->body);
    }
}

void Interpreter::executeFunctionDeclarationStatement(FunctionDeclarationStatement *stmt) {
    environment->define(stmt->name, {ValueType::Object, {.object = (Object*)allocateFunctionObject(stmt)}}, false);
}

void Interpreter::executeReturnStatement(ReturnStatement *stmt) {
    RuntimeValue value = {ValueType::Null};
    if(stmt->value != nullptr){
        value = evaluate(stmt->value);
    }
    throw Return(value);
}

RuntimeValue Interpreter::evaluate(Expression *expr) {
    switch (expr->type) {
        case AstNodeType::BinaryExpression:
            return evaluateBinaryExpression(static_cast<BinaryExpression *>(expr));
        case AstNodeType::LogicalExpression:
            return evaluateLogicalExpression(static_cast<LogicalExpression *>(expr));
        case AstNodeType::UnaryExpression:
            return evaluateUnaryExpression(static_cast<UnaryExpression *>(expr));
        case AstNodeType::NumericLiteralExpression:
            return evaluateNumericLiteralExpression(static_cast<NumericLiteralExpression *>(expr));
        case AstNodeType::BooleanLiteralExpression:
            return evaluateBooleanLiteralExpression(static_cast<BooleanLiteralExpression *>(expr));
        case AstNodeType::NullLiteralExpression:
            return evaluateNullLiteralExpression(static_cast<NullLiteralExpression *>(expr));
        case AstNodeType::StringLiteralExpression:
            return evaluateStringLiteralExpression(static_cast<StringLiteralExpression *>(expr));
        case AstNodeType::GroupingExpression:
            return evaluate((static_cast<GroupingExpression *>(expr))->expr);
        case AstNodeType::VariableExpression:
            return evaluateVariableExpression(static_cast<VariableExpression *>(expr));
        case AstNodeType::AssignmentExpression:
            return evaluateAssignmentExpression(static_cast<AssignmentExpression *>(expr));
        case AstNodeType::CallExpression:
            return evaluateCallExpression(static_cast<CallExpression *>(expr));
        default:
            throw std::runtime_error("Unknown expression type");
    }
}

RuntimeValue Interpreter::evaluateLogicalExpression(LogicalExpression *expr) {
    RuntimeValue left = evaluate(expr->left);

    if (expr->op->type == TokenType::DoublePipe) {
        if (isTruthy(left)) {
            return left;
        }
    } else {
        if (!isTruthy(left)) {
            return left;
        }
    }

    return evaluate(expr->right);
}

RuntimeValue Interpreter::evaluateBinaryExpression(BinaryExpression *expr) {
    RuntimeValue left = evaluate(expr->left);
    RuntimeValue right = evaluate(expr->right);
    switch (expr->op->type) {
        case TokenType::Plus:
            if (left.type == ValueType::Number && right.type == ValueType::Number) {
                return {ValueType::Number, {.number = left.as.number + right.as.number}};
            }
            if (IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) &&
                IS_STRING_OBJ(right)) { // OVDJE ISPOD TREBA NEKI AS_STR MACRO
                return {ValueType::Object, {.object = (Object *) allocateStringObject(
                        GET_STRING_OBJ_VALUE(left) + GET_STRING_OBJ_VALUE(right))}};
            }
            throw WrongBinaryOperandTypes(L"+", left, right, expr);
        case TokenType::Minus:
            if (left.type != ValueType::Number || right.type != ValueType::Number) {
                throw WrongBinaryOperandTypes(L"-", left, right, expr);
            }
            return {ValueType::Number, {.number = left.as.number - right.as.number}};
        case TokenType::Star:
            if (left.type != ValueType::Number || right.type != ValueType::Number) {
                throw WrongBinaryOperandTypes(L"*", left, right, expr);
            }
            return {ValueType::Number, {.number = left.as.number * right.as.number}};
        case TokenType::Slash:
            if (left.type != ValueType::Number || right.type != ValueType::Number) {
                throw WrongBinaryOperandTypes(L"/", left, right, expr);
            }
            return {ValueType::Number, {.number = left.as.number / right.as.number}};
        case TokenType::Percent:
            if (left.type != ValueType::Number || right.type != ValueType::Number) {
                throw WrongBinaryOperandTypes(L"%", left, right, expr);
            }
            return {ValueType::Number, {.number = std::fmod(left.as.number, right.as.number)}};
        case TokenType::Greater:
            if (left.type == ValueType::Number && right.type == ValueType::Number) {
                return {ValueType::Boolean, {.boolean = left.as.number > right.as.number}};
            }
            if (IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)) {
                return {ValueType::Boolean, {.boolean = GET_STRING_OBJ_VALUE(left) > GET_STRING_OBJ_VALUE(right)}};
            }
            if (left.type == ValueType::Boolean && right.type == ValueType::Boolean) {
                return {ValueType::Boolean, {.boolean = left.as.boolean > right.as.boolean}};
            }
            throw WrongBinaryOperandTypes(L">", left, right, expr);
        case TokenType::GreaterEqual:
            if (left.type == ValueType::Number && right.type == ValueType::Number) {
                return {ValueType::Boolean, {.boolean = left.as.number >= right.as.number}};
            }
            if (IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)) {
                return {ValueType::Boolean, {.boolean = GET_STRING_OBJ_VALUE(left) >= GET_STRING_OBJ_VALUE(right)}};
            }
            if (left.type == ValueType::Boolean && right.type == ValueType::Boolean) {
                return {ValueType::Boolean, {.boolean = left.as.boolean >= right.as.boolean}};
            }
            throw WrongBinaryOperandTypes(L">=", left, right, expr);
        case TokenType::Less:
            if (left.type == ValueType::Number && right.type == ValueType::Number) {
                return {ValueType::Boolean, {.boolean = left.as.number < right.as.number}};
            }
            if (IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)) {
                return {ValueType::Boolean, {.boolean = GET_STRING_OBJ_VALUE(left) < GET_STRING_OBJ_VALUE(right)}};
            }
            if (left.type == ValueType::Boolean && right.type == ValueType::Boolean) {
                return {ValueType::Boolean, {.boolean = left.as.boolean < right.as.boolean}};
            }
            throw WrongBinaryOperandTypes(L"<", left, right, expr);
        case TokenType::LessEqual:
            if (left.type == ValueType::Number && right.type == ValueType::Number) {
                return {ValueType::Boolean, {.boolean = left.as.number <= right.as.number}};
            }
            if (IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)) {
                return {ValueType::Boolean, {.boolean = GET_STRING_OBJ_VALUE(left) <= GET_STRING_OBJ_VALUE(right)}};
            }
            if (left.type == ValueType::Boolean && right.type == ValueType::Boolean) {
                return {ValueType::Boolean, {.boolean = left.as.boolean <= right.as.boolean}};
            }
            throw WrongBinaryOperandTypes(L"<=", left, right, expr);
        case TokenType::DoubleEqual:
            return {ValueType::Boolean, {.boolean = isEqual(left, right)}};
        case TokenType::BangEqual:
            return {ValueType::Boolean, {.boolean = !isEqual(left, right)}};
    }
    throw std::runtime_error("Unknown binary operator type!");
}

//
// Moguce ovo bez funkcije samo ubaciti return gore u switch ako se ne bude ovdje vise nista dodavalo
//
RuntimeValue Interpreter::evaluateVariableExpression(VariableExpression *expr) {
    return lookUpVariable(expr);
}

RuntimeValue Interpreter::evaluateAssignmentExpression(AssignmentExpression *expr) {
    RuntimeValue value = evaluate(expr->value);

    auto distance = locals.find(expr);
    if(distance != locals.end()){
        environment->assignAt(distance->second, expr->name->value, value);
    } else {
        globals->assign(expr->name, value);
    }

    return value;
}

RuntimeValue Interpreter::evaluateNumericLiteralExpression(NumericLiteralExpression *expr) {
    return {ValueType::Number, {.number = expr->value}};
}

RuntimeValue Interpreter::evaluateUnaryExpression(UnaryExpression *expr) {
    RuntimeValue value = evaluate(expr->expr);
    switch (expr->op->type) {
        case TokenType::Minus: {
            if (value.type == ValueType::Number) return {ValueType::Number, {.number = -value.as.number}};
            if (value.type == ValueType::Null) return {ValueType::Number, {.number = -0}};
            throw WrongTypeError(L"-", value, expr);
        }
        case TokenType::Bang: {
            return {ValueType::Boolean, {.boolean = !isTruthy(value)}};
        }
        default:
            throw "PARSER ERROR: Unknown unary operator type";
    }
}

RuntimeValue Interpreter::evaluateBooleanLiteralExpression(BooleanLiteralExpression *expr) {
    return {ValueType::Boolean, {.boolean = expr->value}};
}

RuntimeValue Interpreter::evaluateNullLiteralExpression(NullLiteralExpression *expr) {
    return {ValueType::Null};
}

RuntimeValue Interpreter::evaluateStringLiteralExpression(StringLiteralExpression *expr) {
    return {ValueType::Object, {.object = (Object *) (allocateStringObject(expr->value))}};
}

RuntimeValue Interpreter::evaluateCallExpression(CallExpression *expr) {
    RuntimeValue callee = evaluate(expr->callee);

    std::vector<RuntimeValue> arguments;
    for(auto arg: expr->arguments){
        arguments.push_back(evaluate(arg));
    }

    if(!IS_OBJ(callee)){
        throw InvalidCall(callee, getMostRelevantToken(expr->callee));
    }
    if(!IS_CALLABLE_OBJ(callee) && !IS_FUNCTION_OBJ(callee)){
        throw InvalidCall(callee, getMostRelevantToken(expr->callee));
    }

    ObjectCallable* callable = AS_CALLABLE_OBJ(callee);
    if(arguments.size() != callable->arity){
        throw InvalidArgumentCount(callable->arity, arguments.size(), getMostRelevantToken(expr->callee), expr->paren);
    }
    return callable->call(this, arguments);
//    if(callee.as.object->type == ObjectType::OBJECT_CALLABLE){
//    bilo ovjde ovo iznad sto je sad ovo callable = .... do returna, ali izgleda da radi sa castom onim na objectcallable
//    } else {
//        (ObjectFunction*)callee.as.object;
//        if(arguments.size() != ((ObjectFunction*)callee.as.object)->arity){
//            throw "Expected " + std::to_string(((ObjectFunction*)callee.as.object)->arity) + " arguments but got " + std::to_string(arguments.size());
//        }
//        return ((ObjectFunction*)callee.as.object)->functionCall(this, arguments);
//    }
}

bool Interpreter::isTruthy(const RuntimeValue &value) {
    switch (value.type) {
        case ValueType::Boolean:
            return value.as.boolean;
        case ValueType::Number:
            return value.as.number != 0;
        case ValueType::Null:
            return false;
        default:
            return true;
    }
}

bool Interpreter::isEqual(const RuntimeValue &left, const RuntimeValue &right) {
    if (left.type != right.type) return false;
    switch (left.type) {
        case ValueType::Boolean:
            return left.as.boolean == right.as.boolean;
        case ValueType::Number:
            return left.as.number == right.as.number;
        case ValueType::Null:
            return true;
        case ValueType::Object:
            if (IS_STRING_OBJ(left) && IS_STRING_OBJ(right)) {
                return GET_STRING_OBJ_VALUE(left) == GET_STRING_OBJ_VALUE(right);
            }
            throw "EQUALITY NOT YET IMPLEMENTED FOR NON STRING!";
//            return left.as.object == right.as.object;
        default:
            return false;
    }
}

ObjectString *Interpreter::allocateStringObject(const std::wstring &value) {
    auto *obj = new ObjectString();
    obj->obj.type = ObjectType::OBJECT_STRING;
    obj->value = value;
    obj->obj.next = objects;
    objects = (Object *) obj;
    return obj;
}

ObjectFunction *Interpreter::allocateFunctionObject(FunctionDeclarationStatement *declaration) {
    auto *obj = new ObjectFunction(declaration, environment);
    obj->obj.next = objects;
    objects = (Object *) obj;
    return obj;
}

RuntimeError* Interpreter::reallocateError(RuntimeError* error){

    delete handledError; // In case one was already allocated

    if(dynamic_cast<WrongTypeError*>(error) != nullptr){
        handledError = new WrongTypeError(*dynamic_cast<WrongTypeError*>(error));
    } else if(dynamic_cast<WrongBinaryOperandTypes*>(error) != nullptr){
        handledError = new WrongBinaryOperandTypes(*dynamic_cast<WrongBinaryOperandTypes*>(error));
    } else if(dynamic_cast<InvalidCall*>(error) != nullptr){
        handledError = new InvalidCall(*dynamic_cast<InvalidCall*>(error));
    } else if(dynamic_cast<InvalidArgumentCount*>(error) != nullptr){
        handledError = new InvalidArgumentCount(*dynamic_cast<InvalidArgumentCount*>(error));
    } else if(dynamic_cast<UndeclaredVariable*>(error) != nullptr){
        handledError = new UndeclaredVariable(*dynamic_cast<UndeclaredVariable*>(error));
    } else if(dynamic_cast<VariableRedeclaration*>(error) != nullptr){
        handledError = new VariableRedeclaration(*dynamic_cast<VariableRedeclaration*>(error));
    } else if(dynamic_cast<ConstReassignment*>(error) != nullptr){
        handledError = new ConstReassignment(*dynamic_cast<ConstReassignment*>(error));
    } else {
        throw std::runtime_error("ERROR REALLOCATION ERROR: Unknown error type");
    }
    return handledError;
}

RuntimeValue Interpreter::lookUpVariable(const VariableExpression *expr) {
    auto distance = locals.find(expr);
    if(distance != locals.end()){
        return environment->getAt(distance->second, expr->name->value);
    } else {
        return globals->get(expr->name);
    }
}