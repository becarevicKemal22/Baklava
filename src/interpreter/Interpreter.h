//
// Created by kemal on 2/6/2024.
//

#ifndef MATURSKI_2_INTERPRETER_H
#define MATURSKI_2_INTERPRETER_H


#include "RuntimeValue.h"
#include "Ast.h"
#include "ErrorPrinter.h"

class Interpreter {
public:
    Interpreter() : errorPrinter(nullptr) {}
    explicit Interpreter(ErrorPrinter* errorPrinter) : errorPrinter(errorPrinter) {}

    RuntimeValue evaluate(Expression* expr);

    bool hadError = false;
private:
    ErrorPrinter* errorPrinter;

    RuntimeValue evaluateBinaryExpression(BinaryExpression* expr);
    RuntimeValue evaluateUnaryExpression(UnaryExpression* expr);
    RuntimeValue evaluateNumericLiteralExpression(NumericLiteralExpression* expr);
    RuntimeValue evaluateBooleanLiteralExpression(BooleanLiteralExpression* expr);
//    RuntimeValue evaluateStringLiteralExpression(StringLiteralExpression* expr);
    RuntimeValue evaluateNullLiteralExpression(NullLiteralExpression* expr);

    bool isTruthy(const RuntimeValue& value);
};


#endif //MATURSKI_2_INTERPRETER_H
