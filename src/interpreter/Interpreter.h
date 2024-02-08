//
// Created by kemal on 2/6/2024.
//

#ifndef MATURSKI_2_INTERPRETER_H
#define MATURSKI_2_INTERPRETER_H


#include "RuntimeValue.h"
#include "Ast.h"

class Interpreter {
public :
    RuntimeValue* evaluate(Statement* expr);
private:
    RuntimeValue* evaluateBinaryExpression(BinaryExpression* expr);
    RuntimeValue* evaluateUnaryExpression(UnaryExpression* expr);
    RuntimeValue* evaluateNumericLiteralExpression(NumericLiteralExpression* expr);
    RuntimeValue* evaluateBooleanLiteralExpression(BooleanLiteralExpression* expr);
    RuntimeValue* evaluateStringLiteralExpression(StringLiteralExpression* expr);
    RuntimeValue* evaluateNullLiteralExpression(NullLiteralExpression* expr);
};


#endif //MATURSKI_2_INTERPRETER_H
