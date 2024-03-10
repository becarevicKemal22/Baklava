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

#include "WrongTypeError.h"
#include "WrongBinaryOperandTypes.h"

#include <iostream>

RuntimeValue Interpreter::evaluate(Expression *expr) {
    try {
        switch (expr->type) {
            case AstNodeType::BinaryExpression:
                return evaluateBinaryExpression(static_cast<BinaryExpression *>(expr));
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

RuntimeValue Interpreter::evaluateBinaryExpression(BinaryExpression *expr) {
    RuntimeValue left = evaluate(expr->left);
    RuntimeValue right = evaluate(expr->right);
    switch (expr->op->type) {
        case TokenType::Plus:
//            if(left.type == ValueType::String && right.type == ValueType::String){
//                return {ValueType::String, {.string = left.as.string + right.as.string}};
//            }
            if (left.type == ValueType::Number && right.type == ValueType::Number) {
                return {ValueType::Number, {.number = left.as.number + right.as.number}};
            }
            if(left.type == ValueType::Object && left.as.object->type == ObjectType::OBJECT_STRING){
                if(right.type == ValueType::Object && right.as.object->type == ObjectType::OBJECT_STRING){
                    return {ValueType::Object, {.object = (Object*) allocateStringObject(((ObjectString*) left.as.object)->value + ((ObjectString*) right.as.object)->value)}};
                }
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
    }
    throw std::runtime_error("Unknown binary operator type - PARSING BUG!");
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

ObjectString* Interpreter::allocateStringObject(const std::wstring& value) {
    auto* obj = new ObjectString();
    obj->obj.type = ObjectType::OBJECT_STRING;
    obj->value = value;
    obj->obj.next = objects;
    objects = (Object*) obj;
    return obj;
}
