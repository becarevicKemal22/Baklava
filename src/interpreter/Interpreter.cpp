//
// Created by kemal on 2/6/2024.
//

#include "Interpreter.h"
#include "Expression.h"
#include "BinaryExpression.h"
#include "UnaryExpression.h"
#include "NumericLiteralExpression.h"
#include "BooleanLiteralExpression.h"
#include "NullLiteralExpression.h"
#include "StringLiteralExpression.h"
#include "RuntimeValue.h"
#include "LogicalExpression.h"

#include "WrongTypeError.h"
#include "WrongBinaryOperandTypes.h"

#include <iostream>

RuntimeValue Interpreter::evaluate(Expression *expr) {
    try {
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
            default:
                throw std::runtime_error("Unknown expression type");
        }
    } catch (RuntimeError &e) {
        if (errorPrinter != nullptr) errorPrinter->printRuntimeError(&e);
        hadError = true;
        return {ValueType::Null};
    }

}

RuntimeValue Interpreter::evaluateLogicalExpression(LogicalExpression *expr) {
    RuntimeValue left = evaluate(expr->left);

    if(expr->op->type == TokenType::DoublePipe){
        if(isTruthy(left)){
            return left;
        }
    } else {
        if(!isTruthy(left)){
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
            if(IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)){ // OVDJE ISPOD TREBA NEKI AS_STR MACRO
                return {ValueType::Object, {.object = (Object*) allocateStringObject(GET_STRING_OBJ_VALUE(left) + GET_STRING_OBJ_VALUE(right))}};
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
        case TokenType::Greater:
            if(left.type == ValueType::Number && right.type == ValueType::Number){
                return {ValueType::Boolean, {.boolean = left.as.number > right.as.number}};
            }
            if(IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)){
                return {ValueType::Boolean, {.boolean = GET_STRING_OBJ_VALUE(left) > GET_STRING_OBJ_VALUE(right)}};
            }
            if(left.type == ValueType::Boolean && right.type == ValueType::Boolean){
                return {ValueType::Boolean, {.boolean = left.as.boolean > right.as.boolean}};
            }
            throw WrongBinaryOperandTypes(L">", left, right, expr);
        case TokenType::GreaterEqual:
            if(left.type == ValueType::Number && right.type == ValueType::Number){
                return {ValueType::Boolean, {.boolean = left.as.number >= right.as.number}};
            }
            if(IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)){
                return {ValueType::Boolean, {.boolean = GET_STRING_OBJ_VALUE(left) >= GET_STRING_OBJ_VALUE(right)}};
            }
            if(left.type == ValueType::Boolean && right.type == ValueType::Boolean){
                return {ValueType::Boolean, {.boolean = left.as.boolean >= right.as.boolean}};
            }
            throw WrongBinaryOperandTypes(L">=", left, right, expr);
        case TokenType::Less:
            if(left.type == ValueType::Number && right.type == ValueType::Number){
                return {ValueType::Boolean, {.boolean = left.as.number < right.as.number}};
            }
            if(IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)){
                return {ValueType::Boolean, {.boolean = GET_STRING_OBJ_VALUE(left) < GET_STRING_OBJ_VALUE(right)}};
            }
            if(left.type == ValueType::Boolean && right.type == ValueType::Boolean){
                return {ValueType::Boolean, {.boolean = left.as.boolean < right.as.boolean}};
            }
            throw WrongBinaryOperandTypes(L"<", left, right, expr);
        case TokenType::LessEqual:
            if(left.type == ValueType::Number && right.type == ValueType::Number){
                return {ValueType::Boolean, {.boolean = left.as.number <= right.as.number}};
            }
            if(IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)){
                return {ValueType::Boolean, {.boolean = GET_STRING_OBJ_VALUE(left) <= GET_STRING_OBJ_VALUE(right)}};
            }
            if(left.type == ValueType::Boolean && right.type == ValueType::Boolean){
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
    return {ValueType::Object, {.object = (Object *)(allocateStringObject(expr->value))}};
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
            if(IS_STRING_OBJ(left) && IS_STRING_OBJ(right)){
                return GET_STRING_OBJ_VALUE(left) == GET_STRING_OBJ_VALUE(right);
            }
            throw "EQUALITY NOT YET IMPLEMENTED FOR NON STRING!";
//            return left.as.object == right.as.object;
        default:
            return false;
    }
}

ObjectString* Interpreter::allocateStringObject(const std::wstring& value) {
    auto* obj = new ObjectString();
    obj->obj.type = ObjectType::OBJECT_STRING;
    obj->value = value;
    obj->obj.next = objects;
    objects = (Object*) obj;
    return obj;
}
