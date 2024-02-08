//
// Created by kemal on 2/6/2024.
//

#include "Interpreter.h"
#include "Expression.h"
#include "BinaryExpression.h"
#include "UnaryExpression.h"
#include "NumericLiteralExpression.h"
#include "BooleanLiteralExpression.h"
#include "StringLiteralExpression.h"
#include "NullLiteralExpression.h"

#include "NullValue.h"
#include "BooleanValue.h"
#include "StringValue.h"
#include "NumericValue.h"

RuntimeValue* Interpreter::evaluate(Statement* expr) {
    switch (expr->type) {
        case AstNodeType::BinaryExpression:
            return evaluateBinaryExpression((BinaryExpression*)expr);
        case AstNodeType::UnaryExpression:
            return evaluateUnaryExpression((UnaryExpression*)expr);
        case AstNodeType::NumericLiteralExpression:
            return evaluateNumericLiteralExpression((NumericLiteralExpression*)expr);
        case AstNodeType::BooleanLiteralExpression:
            return evaluateBooleanLiteralExpression((BooleanLiteralExpression*)expr);
        case AstNodeType::StringLiteralExpression:
            return evaluateStringLiteralExpression((StringLiteralExpression*)expr);
        case AstNodeType::NullLiteralExpression:
            return evaluateNullLiteralExpression((NullLiteralExpression*)expr);
        default:
            return nullptr;
    }
}

RuntimeValue* Interpreter::evaluateNullLiteralExpression(NullLiteralExpression *expr) {
    return new NullValue();
}

RuntimeValue* Interpreter::evaluateBooleanLiteralExpression(BooleanLiteralExpression *expr) {
    return new BooleanValue(expr->value);
}

RuntimeValue* Interpreter::evaluateNumericLiteralExpression(NumericLiteralExpression *expr) {
    return new NumericValue(expr->value);
}

RuntimeValue* Interpreter::evaluateStringLiteralExpression(StringLiteralExpression *expr) {
    return new StringValue(expr->value);
}

RuntimeValue* Interpreter::evaluateUnaryExpression(UnaryExpression *expr) {
    auto operand = evaluate(expr->expr);
    if (operand->type == ValueType::Null) {
        return operand;
    }
    if (expr->op->type == TokenType::Minus) {
        return new NumericValue(-((NumericValue*)operand)->value);
    }
    if (expr->op->type == TokenType::Bang) {
        return new BooleanValue(!((BooleanValue*)operand)->value);
    }
    return nullptr;
}

RuntimeValue* Interpreter::evaluateBinaryExpression(BinaryExpression *expr) {
    auto left = evaluate(expr->left);
    auto right = evaluate(expr->right);
    if (left->type == ValueType::Null || right->type == ValueType::Null) {
        return new NullValue();
    }
    if (expr->op->type == TokenType::Plus) {
        if (left->type == ValueType::String || right->type == ValueType::String) {
            return new StringValue(((StringValue*)left)->value + ((StringValue*)right)->value);
        }
        return new NumericValue(((NumericValue*)left)->value + ((NumericValue*)right)->value);
    }
    if (expr->op->type == TokenType::Minus) {
        return new NumericValue(((NumericValue*)left)->value - ((NumericValue*)right)->value);
    }
    if (expr->op->type == TokenType::Star) {
        return new NumericValue(((NumericValue*)left)->value * ((NumericValue*)right)->value);
    }
    if (expr->op->type == TokenType::Slash) {
        return new NumericValue(((NumericValue*)left)->value / ((NumericValue*)right)->value);
    }
    if (expr->op->type == TokenType::DoubleEqual) {
        return new BooleanValue(((NumericValue*)left)->value == ((NumericValue*)right)->value);
    }
    if (expr->op->type == TokenType::BangEqual) {
        return new BooleanValue(((NumericValue*)left)->value != ((NumericValue*)right)->value);
    }
    if (expr->op->type == TokenType::Greater) {
        return new BooleanValue(((NumericValue*)left)->value > ((NumericValue*)right)->value);
    }
    if (expr->op->type == TokenType::GreaterEqual) {
        return new BooleanValue(((NumericValue*)left)->value >= ((NumericValue*)right)->value);
    }
    if (expr->op->type == TokenType::Less) {
        return new BooleanValue(((NumericValue*)left)->value < ((NumericValue*)right)->value);
    }
    if (expr->op->type == TokenType::LessEqual) {
        return new BooleanValue(((NumericValue*)left)->value <= ((NumericValue*)right)->value);
    }
    throw "ERRROR";
}
