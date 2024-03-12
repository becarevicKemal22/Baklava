//
// Created by kemal on 2/6/2024.
//

#ifndef MATURSKI_2_INTERPRETER_H
#define MATURSKI_2_INTERPRETER_H


#include "RuntimeValue.h"
#include "Ast.h"
#include "ErrorPrinter.h"

/**
 * @brief Class that runs a program.
 *
 * The Interpreter class is responsible for evaluating the AST and running the program, as well as handling errors, keeping track of the environment, etc.
 */
class Interpreter {
public:
    /**
     * @brief Constructor with no error printer. Should almost never be used, as it leaves the interpreter with no error printing capability.
     */
    Interpreter() : errorPrinter(nullptr) {}
    /**
     * @brief Constructor with an error printer. Recommended constructor.
     * @param errorPrinter Error printer to use for printing errors.
     */
    explicit Interpreter(ErrorPrinter* errorPrinter) : errorPrinter(errorPrinter) {}

    /**
     * @brief Evaluates the given expression.
     * @param expr Expression to evaluate.
     * @return RuntimeValue that holds the result of the expression evaluation.
     * @throws RuntimeError Throws RuntimeError and its derived classes if an error occurs during evaluation.
     */
    RuntimeValue evaluate(Expression* expr);

    /**
     * @brief Executes the given statement.
     * @param stmt Statement to execute.
     * @throws RuntimeError Throws RuntimeError and its derived classes if an error occurs during execution.
     */
    void execute(Statement* stmt);

    /**
     * @brief Interprets the given program.
     * @param program Program to interpret.
     */
    void interpret(Program* program);

//    ~Interpreter(){
//        if(objects != nullptr){
//            Object* object = objects;
//            while(object != nullptr){
//                Object* next = object->next;
//                delete object;
//                object = next;
//            }
//        }
//    }

    bool hadError = false; /**< Holds whether an error has occurred during the interpretation. Interpretation should stop if this is set to true. */
private:
    ErrorPrinter* errorPrinter;

    void executeExpressionStatement(ExpressionStatement* stmt);
    void executePrintStatement(PrintStatement* stmt);

    RuntimeValue evaluateBinaryExpression(BinaryExpression* expr);
    RuntimeValue evaluateLogicalExpression(LogicalExpression* expr);
    RuntimeValue evaluateUnaryExpression(UnaryExpression* expr);
    RuntimeValue evaluateNumericLiteralExpression(NumericLiteralExpression* expr);
    RuntimeValue evaluateBooleanLiteralExpression(BooleanLiteralExpression* expr);
    RuntimeValue evaluateStringLiteralExpression(StringLiteralExpression* expr);
    RuntimeValue evaluateNullLiteralExpression(NullLiteralExpression* expr);

    bool isTruthy(const RuntimeValue& value);
    bool isEqual(const RuntimeValue& left, const RuntimeValue& right);

    Object* objects = nullptr;

    ObjectString* allocateStringObject(const std::wstring& value);
};


#endif //MATURSKI_2_INTERPRETER_H
