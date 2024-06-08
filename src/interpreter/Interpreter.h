//
// Created by kemal on 2/6/2024.
//

#ifndef BAKLAVA_INTERPRETER_H
#define BAKLAVA_INTERPRETER_H

#include <stack>

#include "Object.h"
#include "Ast.h"
#include "ErrorPrinter.h"
#include "Environment.h"
#include "AssignmentExpression.h"

/**
 * @brief Class that runs a program.
 *
 * The Interpreter class is responsible for evaluating the AST and running the program, as well as handling errors, keeping track of the environment, etc.
 */
class Interpreter {
public:
    /**
     * @brief Constructor with no error printer. Should almost never be used, as it leaves the interpreter with no error printing capability. Useful for testing.
     */
    Interpreter() : errorPrinter(nullptr) {
        environments.emplace();
        globals = &environments.top();
        defineNativeFunctions();
    }

    /**
     * @brief Constructor with an error printer. Recommended constructor.
     * @param errorPrinter Error printer to use for printing errors.
     */
    explicit Interpreter(ErrorPrinter *errorPrinter) : errorPrinter(errorPrinter) {
        environments.emplace();
        globals = &environments.top();
        defineNativeFunctions();
    }

    void defineNativeFunctions();

    ~Interpreter() {
//        if(objects != nullptr){
//            Object* object = objects;
//            while(object != nullptr){
//                Object* next = object->next;
//                delete object;
//                object = next;
//            }
//        }
    }

    /**
     * @brief Evaluates the given expression.
     * @param expr Expression to evaluate.
     * @return RuntimeValue that holds the result of the expression evaluation.
     * @throws RuntimeError Throws RuntimeError and its derived classes if an error occurs during evaluation.
     */
    RuntimeValue evaluate(Expression *expr);

    /**
     * @brief Executes the given statement.
     * @param stmt Statement to execute.
     * @throws RuntimeError Throws RuntimeError and its derived classes if an error occurs during execution.
     */
    void execute(Statement *stmt);

    /**
     * @brief Interprets the given program.
     * @param program Program to interpret.
     */
    void interpret(Program *program);

    void resolve(const Expression *expr, int depth){
        locals[expr] = depth;
    }

    bool hadError = false; /**< Holds whether an error has occurred during the interpretation. Interpretation should stop if this is set to true. */
    Environment *globals;

    std::vector<Statement *> executedStatements; /**< List of pointers to all statements that have been executed. Only written to when DEBUG_TRACK_EXECUTION is set */
    std::vector<RuntimeValue> printHistory; /**< List of all values that have been printed. Only written to when DEBUG_TRACK_PRINTING is set */
    RuntimeError* handledError = nullptr; /**< Holds the last error that has been handled. No flags needed as this is only done when the program has to quit so performance is not an issue.*/

    void executeBlock(const std::vector<StmtPtr> &statements, const Environment& environment);

    RuntimeValue returnedValue;
    bool isReturning = false;

    std::stack<Environment> environments;
private:
    std::unordered_map<const Expression*, int> locals;

    ErrorPrinter *errorPrinter;

    RuntimeValue lookUpVariable(const VariableExpression *expr);

    void executeExpressionStatement(ExpressionStatement *stmt);

    void executePrintStatement(PrintStatement *stmt);

    void executeVarDeclarationStatement(VarDeclarationStatement *stmt);

    void executeBlockStatement(BlockStatement *stmt);

    void executeIfStatement(IfStatement *stmt);

    void executeWhileStatement(WhileStatement *stmt);

    void executeFunctionDeclarationStatement(FunctionDeclarationStatement *stmt);

    void executeReturnStatement(ReturnStatement *stmt);

    RuntimeValue evaluateBinaryExpression(BinaryExpression *expr);

    RuntimeValue evaluateLogicalExpression(LogicalExpression *expr);

    RuntimeValue evaluateUnaryExpression(UnaryExpression *expr);

    RuntimeValue evaluateNumericLiteralExpression(NumericLiteralExpression *expr);

    RuntimeValue evaluateBooleanLiteralExpression(BooleanLiteralExpression *expr);

    RuntimeValue evaluateStringLiteralExpression(StringLiteralExpression *expr);

    RuntimeValue evaluateNullLiteralExpression(NullLiteralExpression *expr);

    RuntimeValue evaluateVariableExpression(VariableExpression *expr);

    RuntimeValue evaluateAssignmentExpression(AssignmentExpression *expr);

    RuntimeValue evaluateCallExpression(CallExpression *expr);

    RuntimeValue evaluateArrayLiteralExpression(ArrayLiteralExpression *expr);

    RuntimeValue evaluateIndexingExpression(IndexingExpression *expr);

    RuntimeValue evaluateIndexAssignmentExpression(IndexAssignmentExpression *expr);

    bool isTruthy(const RuntimeValue &value);

    bool isEqual(const RuntimeValue &left, const RuntimeValue &right);

    Object *objects = nullptr;

    ObjectString *allocateStringObject(const std::wstring &value);
    ObjectFunction *allocateFunctionObject(FunctionDeclarationStatement *declaration);
    ObjectArray *allocateArrayObject(const std::vector<RuntimeValue> &elements);

    // Used to ensure that errors don't go out of scope when running tests, since the type needs to be checked on the handledError field.
    // This function just dynamically allocates a new error of the same type and returns it (so that handledError can be set and checked in tests).
    RuntimeError* reallocateError(RuntimeError* error);
};


#endif //BAKLAVA_INTERPRETER_H
