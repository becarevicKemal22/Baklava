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
#include "RuntimeValue.h"

#include <iostream>

RuntimeValue Interpreter::evaluate(Expression* expr) {
    switch(expr->type){
        case AstNodeType::BinaryExpression:
            return evaluateBinaryExpression(static_cast<BinaryExpression*>(expr));
        case AstNodeType::UnaryExpression:
            return evaluateUnaryExpression(static_cast<UnaryExpression*>(expr));
        case AstNodeType::NumericLiteralExpression:
            return evaluateNumericLiteralExpression(static_cast<NumericLiteralExpression*>(expr));
        case AstNodeType::BooleanLiteralExpression:
            return evaluateBooleanLiteralExpression(static_cast<BooleanLiteralExpression*>(expr));
        case AstNodeType::NullLiteralExpression:
            return evaluateNullLiteralExpression(static_cast<NullLiteralExpression*>(expr));
        default:
            throw std::runtime_error("Unknown expression type");
    }
}

RuntimeValue Interpreter::evaluateBinaryExpression(BinaryExpression *expr) {
    RuntimeValue left = evaluate(expr->left);
    RuntimeValue right = evaluate(expr->right);
    switch(expr->op->type){
        case TokenType::Plus:
            return {ValueType::Number, {.number = left.as.number + right.as.number}};
        case TokenType::Minus:
            return {ValueType::Number, {.number = left.as.number - right.as.number}};
        case TokenType::Star:
            return {ValueType::Number, {.number = left.as.number * right.as.number}};
        case TokenType::Slash:
            return {ValueType::Number, {.number = left.as.number / right.as.number}};
    }
    throw "ERRRRRRR";
}

RuntimeValue Interpreter::evaluateNumericLiteralExpression(NumericLiteralExpression *expr) {
    return {ValueType::Number, {.number = expr->value}};
}

RuntimeValue Interpreter::evaluateUnaryExpression(UnaryExpression *expr) {
    RuntimeValue value = evaluate(expr->expr);
    switch(expr->op->type){
        case TokenType::Minus: {
            if(value.type == ValueType::Number) return {ValueType::Number, {.number = -value.as.number}};
            if(value.type == ValueType::Null) return {ValueType::Number, {.number = -0}};
            if(value.type == ValueType::Boolean) return {ValueType::Number, {.number = static_cast<double>(-value.as.boolean)}};
            break;
        }
        case TokenType::Bang: {
            std::wcout << "Evaluating bang" << std::endl;
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

bool Interpreter::isTruthy(const RuntimeValue &value) {
    switch(value.type){
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
