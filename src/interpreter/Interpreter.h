//
// Created by kemal on 2/6/2024.
//

#ifndef MATURSKI_2_INTERPRETER_H
#define MATURSKI_2_INTERPRETER_H


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
     * @brief Constructor with no error printer. Should almost never be used, as it leaves the interpreter with no error printing capability.
     */
    Interpreter() : errorPrinter(nullptr) {
        globals = new Environment();
        defineNativeFunctions();
        environment = globals;
    }

    /**
     * @brief Constructor with an error printer. Recommended constructor.
     * @param errorPrinter Error printer to use for printing errors.
     */
    explicit Interpreter(ErrorPrinter *errorPrinter) : errorPrinter(errorPrinter) {
        globals = new Environment();
        defineNativeFunctions();
        environment = globals;
    }

    void defineNativeFunctions() {
        RuntimeValue clockFunction;
        clockFunction.type = ValueType::Object;
        auto *clockCallable = new ObjectCallable();
        clockCallable->obj.type = ObjectType::OBJECT_CALLABLE;
        clockCallable->arity = 0;
        clockCallable->call = [](Interpreter *interpreter, const std::vector<RuntimeValue> &arguments) -> RuntimeValue {
            return {ValueType::Number, {.number = (double) clock() / CLOCKS_PER_SEC}};
        };

        clockFunction.as.object = (Object *) clockCallable;

        auto clockToken = new Token(TokenType::Identifier, L"clock", 0, 0);
        globals->define(clockToken, clockFunction, true);

        RuntimeValue emptyFunction;
        emptyFunction.type = ValueType::Object;
        ObjectCallable *emptyCallable = new ObjectCallable();
        emptyCallable->obj.type = ObjectType::OBJECT_CALLABLE;
        emptyCallable->arity = 0;
        emptyCallable->call = [](Interpreter *interpreter, const std::vector<RuntimeValue> &arguments) -> RuntimeValue {
            return {ValueType::Null};
        };

        emptyFunction.as.object = (Object *) emptyCallable;

        auto emptyToken = new Token(TokenType::Identifier, L"mockFunkcija", 0, 0);
        globals->define(emptyToken, emptyFunction, true);

    }

    ~Interpreter() {
//        if(objects != nullptr){
//            Object* object = objects;
//            while(object != nullptr){
//                Object* next = object->next;
//                delete object;
//                object = next;
//            }
//        }
        delete environment;
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

    bool hadError = false; /**< Holds whether an error has occurred during the interpretation. Interpretation should stop if this is set to true. */
    Environment *globals;
    Environment *environment;

    std::vector<Statement *> executedStatements; /**< List of pointers to all statements that have been executed. Only written to when DEBUG_TRACK_EXECUTION is set */
    std::vector<RuntimeValue> printHistory; /**< List of all values that have been printed. Only written to when DEBUG_TRACK_PRINTING is set */
    RuntimeError* handledError = nullptr; /**< Holds the last error that has been handled. No flags needed as this is only done when the program has to quit so performance is not an issue.*/

    void executeBlock(const std::vector<StmtPtr> &statements, Environment *environment);
private:
    ErrorPrinter *errorPrinter;

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

    bool isTruthy(const RuntimeValue &value);

    bool isEqual(const RuntimeValue &left, const RuntimeValue &right);

    Object *objects = nullptr;

    ObjectString *allocateStringObject(const std::wstring &value);
    ObjectFunction *allocateFunctionObject(FunctionDeclarationStatement *declaration);

    // Used to ensure that errors don't go out of scope when running tests, since the type needs to be checked on the handledError field.
    // This function just dynamically allocates a new error of the same type and returns it (so that handledError can be set and checked in tests).
    RuntimeError* reallocateError(RuntimeError* error);
};


#endif //MATURSKI_2_INTERPRETER_H
