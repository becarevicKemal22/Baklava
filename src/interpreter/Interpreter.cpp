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

RuntimeValue Interpreter::evaluate(Statement *expr) {
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
            return {ValueType::NUMBER, {.number = left.as.number + right.as.number}};
        case TokenType::Minus:
            return {ValueType::NUMBER, {.number = left.as.number - right.as.number}};
        case TokenType::Star:
            return {ValueType::NUMBER, {.number = left.as.number * right.as.number}};
        case TokenType::Slash:
            return {ValueType::NUMBER, {.number = left.as.number / right.as.number}};
    }
    throw "ERRRRRRR";
}

RuntimeValue Interpreter::evaluateNumericLiteralExpression(NumericLiteralExpression *expr) {
    return {ValueType::NUMBER, {.number = expr->value}};
}

RuntimeValue Interpreter::evaluateUnaryExpression(UnaryExpression *expr) {
    RuntimeValue value = evaluate(expr->expr);
    switch(expr->op->type){
        case TokenType::Minus:
            return {ValueType::NUMBER, {.number = -value.as.number}};
    }
}

RuntimeValue Interpreter::evaluateBooleanLiteralExpression(BooleanLiteralExpression *expr) {}

RuntimeValue Interpreter::evaluateNullLiteralExpression(NullLiteralExpression *expr) {}
