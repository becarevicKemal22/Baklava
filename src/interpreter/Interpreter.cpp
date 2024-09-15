//
// Created by kemal on 2/6/2024.
//

/*
 * Listed below are the flags that can be set to alter the behaviour of the interpreter.
 *
 * DEBUG_STRESS_GC: If set, the garbage collector will be invoked after every object allocation (excluding arrays).
 * DEBUG_LOG_GC: Has two levels, the first level just logs that the garbage collector has been invoked, as well as the
 *               amount of memory that has been allocated / freed and the new limit of memory that can be allocated before
 *               gc is run again. The second level additionally logs the entire process during marking and tracing,
 *               including the identifiers and addresses of objects that have been freed, marked, etc. It is only necessary
 *               to enable one of them, as the level 2 flag will enable all the logging that the level 1 flag does.
 * DEBUG_TRACK_EXECUTION: If set, the interpreter will store all executed statement nodes in a vector. Main intended
 *               purpose is for interpreter tests.
 * DEBUG_TRACK_PRINTING: If set, the interpreter will store all printed values in a vector. Also intended for testing,
 *               so that the output of the program can be checked in tests that would otherwise be difficult to inspect.
 */

//#define DEBUG_STRESS_GC
//#define DEBUG_LOG_GC 1
//#define DEBUG_LOG_GC 2
//#define DEBUG_TRACK_EXin ECUTION
//#define DEBUG_TRACK_PRINTING


#include "Interpreter.h"
#include "Program.h"
#include "Expression.h"
#include "BinaryExpression.h"
#include "UnaryExpression.h"
#include "NumericLiteralExpression.h"
#include "BooleanLiteralExpression.h"
#include "NullLiteralExpression.h"
#include "StringLiteralExpression.h"
#include "RuntimeValue.h"
#include "LogicalExpression.h"
#include "ExpressionStatement.h"
#include "PrintStatement.h"
#include "VarDeclarationStatement.h"
#include "VariableExpression.h"
#include "BlockStatement.h"
#include "IfStatement.h"
#include "WhileStatement.h"
#include "CallExpression.h"
#include "ArrayLiteralExpression.h"
#include "IndexingExpression.h"
#include "IndexAssignmentExpression.h"

#include "BaseFunctions.h"

#include "WrongTypeError.h"
#include "WrongBinaryOperandTypes.h"
#include "GroupingExpression.h"
#include "FunctionDeclarationStatement.h"
#include "InvalidCall.h"
#include "InvalidArgumentCount.h"
#include "RuntimeValue.h"
#include "ReturnStatement.h"
#include "Resolver.h"
#include "IndexOutOfBounds.h"
#include "NonIntegerIndex.h"
#include "IndexingNonArray.h"

#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>

void Interpreter::defineNativeFunctions() {
    for(auto& nativeFunction : BaseFunctions::getFunctions(this)){
        globals->define(nativeFunction.name, nativeFunction.function, true);
    }
}

void Interpreter::interpret(Program *program) {
    for (auto stmt: program->statements) {
        try {
            execute(stmt);
        } catch (RuntimeError &e) {
            if (errorPrinter != nullptr) errorPrinter->printRuntimeError(&e);
            hadError = true;
            handledError = reallocateError(&e);
            return;
        }
    }
}

void Interpreter::execute(Statement *stmt) {
#ifdef DEBUG_TRACK_EXECUTION
    executedStatements.push_back(stmt);
#endif
    switch (stmt->type) {
        case AstNodeType::ExpressionStatement:
            executeExpressionStatement(static_cast<ExpressionStatement *>(stmt));
            return;
        case AstNodeType::PrintStatement:
            executePrintStatement(static_cast<PrintStatement *>(stmt));
            return;
        case AstNodeType::VarDeclarationStatement:
            executeVarDeclarationStatement(static_cast<VarDeclarationStatement *>(stmt));
            return;
        case AstNodeType::BlockStatement:
            executeBlockStatement(static_cast<BlockStatement *>(stmt));
            return;
        case AstNodeType::IfStatement:
            executeIfStatement(static_cast<IfStatement *>(stmt));
            return;
        case AstNodeType::WhileStatement:
            executeWhileStatement(static_cast<WhileStatement *>(stmt));
            return;
        case AstNodeType::FunctionDeclarationStatement:
            executeFunctionDeclarationStatement(static_cast<FunctionDeclarationStatement *>(stmt));
            return;
        case AstNodeType::ReturnStatement:
            executeReturnStatement(static_cast<ReturnStatement *>(stmt));
            return;
        default:
            throw std::runtime_error("Unknown statement type");
    }
}

void Interpreter::executeExpressionStatement(ExpressionStatement *stmt) {
    evaluate(stmt->expr);
}

void Interpreter::executePrintStatement(PrintStatement *stmt) {
    RuntimeValue value = evaluate(stmt->expr);
#ifdef DEBUG_TRACK_PRINTING
    printHistory.push_back(value);
#endif
    printValue(value);
    std::wcout << std::endl;
}

void Interpreter::printValue(const RuntimeValue &value, std::wostream &os) {
    switch (value.type) {
        case ValueType::Number:
            os << value.as.number;
            return;
        case ValueType::Boolean:
            os << (value.as.boolean ? L"tačno" : L"netačno");
            return;
        case ValueType::Null:
            os << L"null";
            return;
        case ValueType::Object:
            if (IS_STRING_OBJ(value)) {
                os << GET_STRING_OBJ_VALUE(value);
                return;
            } else if (IS_ARRAY_OBJ(value)) {
                os << L"[";
                const auto &elements = GET_ARRAY_OBJ_ELEMENTS(value);
                for (const auto &element: elements) {
                    printValue(element);
                    if (&element != &elements.back()) {
                        os << L", ";
                    }
                }
                os << L"]";
                return;
            } else if (IS_FUNCTION_OBJ(value)) {
                os << L"<funkcija ";
                os << AS_FUNCTION_OBJ(value)->declaration->name->value;
                os << L">";
                return;
            } else if (IS_CALLABLE_OBJ(value)) {
                os << L"<funkcija>";
                return;
            }
            throw "PRINT NOT YET IMPLEMENTED FOR THIS OBJECT TYPE!";
        default:
            throw "UNKNOWN TYPE TO PRINT";
    }
}

void Interpreter::executeVarDeclarationStatement(VarDeclarationStatement *stmt) {
    RuntimeValue value;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer);
    } else {
        value = {ValueType::Null};
    }

    environments.top().define(stmt->name, value, stmt->isConst);
}

void Interpreter::executeBlockStatement(BlockStatement *stmt) {
    executeBlock(stmt->statements, Environment(&environments.top()));
}

void Interpreter::executeBlock(const std::vector<Statement *> &statements, const Environment &environment) {
    environments.push(environment);
    try {
        for (auto s: statements) {
            execute(s);
            if (isReturning) {
                break;
            }
        }
    } catch (RuntimeError &e) {
        environments.pop();
        throw;
    }
    environments.pop();
}

void Interpreter::executeIfStatement(IfStatement *stmt) {
    if (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->thenBranch);
    } else if (stmt->elseBranch != nullptr) {
        execute(stmt->elseBranch);
    }
}

void Interpreter::executeWhileStatement(WhileStatement *stmt) {
    while (isTruthy(evaluate(stmt->condition)) && !isReturning) {
        execute(stmt->body);
    }
}

void Interpreter::executeFunctionDeclarationStatement(FunctionDeclarationStatement *stmt) {
    environments.top().define(stmt->name, {ValueType::Object, {.object = (Object *) allocateFunctionObject(stmt)}},
                              false);
}

void Interpreter::executeReturnStatement(ReturnStatement *stmt) {
    RuntimeValue value = {ValueType::Null};
    if (stmt->value != nullptr) {
        value = evaluate(stmt->value);
    }
    returnedValue = value;
    isReturning = true;
}

RuntimeValue Interpreter::evaluate(Expression *expr) {
    switch (expr->type) {
        case AstNodeType::BinaryExpression: {
            disallowGC = true; // trenutno nisam siguran da li je moguce da dodje do problema zbog brisanja objekata tokom izvrsavanja nizanih binarnih operacija tipa konkatenacije stringova tako da je najbolje da bude disallowed jer onda nema sanse da ce se izbrisati nesto sto ne treba tokom izvrsavanja binarnih operacija.
            RuntimeValue result = evaluateBinaryExpression(static_cast<BinaryExpression *>(expr));
            disallowGC = false;
            return result;
        }
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
        case AstNodeType::GroupingExpression:
            return evaluate((static_cast<GroupingExpression *>(expr))->expr);
        case AstNodeType::VariableExpression:
            return evaluateVariableExpression(static_cast<VariableExpression *>(expr));
        case AstNodeType::AssignmentExpression:
            return evaluateAssignmentExpression(static_cast<AssignmentExpression *>(expr));
        case AstNodeType::CallExpression:
            return evaluateCallExpression(static_cast<CallExpression *>(expr));
        case AstNodeType::ArrayLiteralExpression:
            return evaluateArrayLiteralExpression(static_cast<ArrayLiteralExpression *>(expr));
        case AstNodeType::IndexingExpression:
            return evaluateIndexingExpression(static_cast<IndexingExpression *>(expr));
        case AstNodeType::IndexAssignmentExpression:
            return evaluateIndexAssignmentExpression(static_cast<IndexAssignmentExpression *>(expr));
        default:
            throw std::runtime_error("Unknown expression type in interpreter");
    }
}

RuntimeValue Interpreter::evaluateLogicalExpression(LogicalExpression *expr) {
    RuntimeValue left = evaluate(expr->left);

    if (expr->op->type == TokenType::DoublePipe) {
        if (isTruthy(left)) {
            return left;
        }
    } else {
        if (!isTruthy(left)) {
            return left;
        }
    }

    return evaluate(expr->right);
}

RuntimeValue Interpreter::evaluateBinaryExpression(BinaryExpression *expr) {
    // GC gets disallowed in general evaluate expression switch
    RuntimeValue left = evaluate(expr->left);
    RuntimeValue right = evaluate(expr->right);
    //maybe make left and right interpreter attributes like returned value so that they are always kept until another expression is done. Maybe not the most effective or efficient but it works, since it will make sure that the values wont get deleted. Check if it is actually true tho, especially for chained binary expressions.
    // actually i think it wont because if right is a binary expression then it will be free to delete the last left / the one in the current binary expr, but it will keep the other one that may even get discarded.
    // seems to be better just to pause GC until binary expression yields its final result.
    switch (expr->op->type) {
        case TokenType::Plus:
            if (left.type == ValueType::Number && right.type == ValueType::Number) {
                return {ValueType::Number, {.number = left.as.number + right.as.number}};
            }
            if (IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) &&
                IS_STRING_OBJ(right)) { // OVDJE ISPOD TREBA NEKI AS_STR MACRO
                return {ValueType::Object, {.object = (Object *) allocateStringObject(
                        GET_STRING_OBJ_VALUE(left) + GET_STRING_OBJ_VALUE(right))}};
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
        case TokenType::Percent:
            if (left.type != ValueType::Number || right.type != ValueType::Number) {
                throw WrongBinaryOperandTypes(L"%", left, right, expr);
            }
            return {ValueType::Number, {.number = std::fmod(left.as.number, right.as.number)}};
        case TokenType::Greater:
            if (left.type == ValueType::Number && right.type == ValueType::Number) {
                return {ValueType::Boolean, {.boolean = left.as.number > right.as.number}};
            }
            if (IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)) {
                return {ValueType::Boolean, {.boolean = GET_STRING_OBJ_VALUE(left) > GET_STRING_OBJ_VALUE(right)}};
            }
            if (left.type == ValueType::Boolean && right.type == ValueType::Boolean) {
                return {ValueType::Boolean, {.boolean = left.as.boolean > right.as.boolean}};
            }
            throw WrongBinaryOperandTypes(L">", left, right, expr);
        case TokenType::GreaterEqual:
            if (left.type == ValueType::Number && right.type == ValueType::Number) {
                return {ValueType::Boolean, {.boolean = left.as.number >= right.as.number}};
            }
            if (IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)) {
                return {ValueType::Boolean, {.boolean = GET_STRING_OBJ_VALUE(left) >= GET_STRING_OBJ_VALUE(right)}};
            }
            if (left.type == ValueType::Boolean && right.type == ValueType::Boolean) {
                return {ValueType::Boolean, {.boolean = left.as.boolean >= right.as.boolean}};
            }
            throw WrongBinaryOperandTypes(L">=", left, right, expr);
        case TokenType::Less:
            if (left.type == ValueType::Number && right.type == ValueType::Number) {
                return {ValueType::Boolean, {.boolean = left.as.number < right.as.number}};
            }
            if (IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)) {
                return {ValueType::Boolean, {.boolean = GET_STRING_OBJ_VALUE(left) < GET_STRING_OBJ_VALUE(right)}};
            }
            if (left.type == ValueType::Boolean && right.type == ValueType::Boolean) {
                return {ValueType::Boolean, {.boolean = left.as.boolean < right.as.boolean}};
            }
            throw WrongBinaryOperandTypes(L"<", left, right, expr);
        case TokenType::LessEqual:
            if (left.type == ValueType::Number && right.type == ValueType::Number) {
                return {ValueType::Boolean, {.boolean = left.as.number <= right.as.number}};
            }
            if (IS_OBJ(left) && IS_STRING_OBJ(left) && IS_OBJ(right) && IS_STRING_OBJ(right)) {
                return {ValueType::Boolean, {.boolean = GET_STRING_OBJ_VALUE(left) <= GET_STRING_OBJ_VALUE(right)}};
            }
            if (left.type == ValueType::Boolean && right.type == ValueType::Boolean) {
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

//
// Moguce ovo bez funkcije samo ubaciti return gore u switch ako se ne bude ovdje vise nista dodavalo
//
RuntimeValue Interpreter::evaluateVariableExpression(VariableExpression *expr) {
    return lookUpVariable(expr);
}

RuntimeValue Interpreter::evaluateAssignmentExpression(AssignmentExpression *expr) {
    RuntimeValue value = evaluate(expr->value);

    auto distance = locals.find(expr);
    if (distance != locals.end()) {
        environments.top().assignAt(distance->second, expr->name->value, value);
    } else {
        globals->assign(expr->name, value);
    }

    return value;
}

RuntimeValue Interpreter::evaluateIndexAssignmentExpression(IndexAssignmentExpression *expr) {

    TokenPtr name = nullptr;
    if (expr->left->type == AstNodeType::VariableExpression) {
        name = static_cast<VariableExpression *>(expr->left)->name;
    }

    RuntimeValue array = evaluate(expr->left);
    RuntimeValue index = evaluate(expr->index);
    RuntimeValue value = evaluate(expr->value);

    // it needs to be || here because a segfault occurs when trying to check if an obj is an array when it is not
    // an object to begin with. || short circuits it, so it doesn't check the second part if the first is true, but still
    // fails if it is a non array object.
    if (!IS_OBJ(array) || !IS_ARRAY_OBJ(array)) {
        throw IndexingNonArray(expr, array);
    }
    if (index.type != ValueType::Number) {
        throw WrongTypeError(L"[]", index, expr->index);
    }

    auto elements = GET_ARRAY_OBJ_ELEMENTS(array);
    if (index.as.number < 0 || index.as.number >= elements.size()) {
        throw IndexOutOfBounds(expr->index, index.as.number);
    }

    if (index.as.number != (int) index.as.number) {
        throw NonIntegerIndex(expr->index, index.as.number);
    }
    AS_ARRAY_OBJ(
            array)->elements[(size_t) index.as.number] = value; // array is modified in place, so that the actual array or its memory location is not changed.

//    auto distance = locals.find(expr);
//    if(distance != locals.end()){
//        environments.top().assignAt(distance->second, name->value, elements);
//    } else {
//        globals->assign(name, elements);
//    } ne modifikuje se varijabla ustvari, vec samo element niza "in place", pogotovo bitno ako ce nizovi biti prenosivi po referenci

    return value;

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
            // This doesnt work? Check if it even exists in parser?
        case TokenType::DoublePlus: {
            if (value.type != ValueType::Number) throw WrongTypeError(L"++", value, expr);
            return {ValueType::Number, {.number = value.as.number + 1}};
        }
        case TokenType::DoubleMinus: {
            if (value.type != ValueType::Number) throw WrongTypeError(L"--", value, expr);
            return {ValueType::Number, {.number = value.as.number - 1}};
        } // PROBLEM JE STO KOD UNARNIH SE PRVO EVALUIRA VRIJEDNOST A ONDA DODJELJUJE AKO JE VARIJABLA TO NE VALJA
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
    return {ValueType::Object, {.object = (Object *) (allocateStringObject(expr->value))}};
}

RuntimeValue Interpreter::evaluateCallExpression(CallExpression *expr) {
    RuntimeValue callee = evaluate(expr->callee);

    disallowGC = true;
    std::vector<RuntimeValue> arguments;
    for (auto arg: expr->arguments) {
        arguments.push_back(evaluate(arg));
    }
    disallowGC = false;

    if (!IS_OBJ(callee)) {
        throw InvalidCall(callee, getMostRelevantToken(expr->callee));
    }
    if (!IS_CALLABLE_OBJ(callee) && !IS_FUNCTION_OBJ(callee)) {
        throw InvalidCall(callee, getMostRelevantToken(expr->callee));
    }

    ObjectCallable *callable = AS_CALLABLE_OBJ(callee);
    if (arguments.size() != callable->arity) {
        throw InvalidArgumentCount(callable->arity, arguments.size(), getMostRelevantToken(expr->callee), expr->paren);
    }
    return callable->call(this, arguments);
//    if(callee.as.object->type == ObjectType::OBJECT_CALLABLE){
//    bilo ovjde ovo iznad sto je sad ovo callable = .... do returna, ali izgleda da radi sa castom onim na objectcallable
//    } else {
//        (ObjectFunction*)callee.as.object;
//        if(arguments.size() != ((ObjectFunction*)callee.as.object)->arity){
//            throw "Expected " + std::to_string(((ObjectFunction*)callee.as.object)->arity) + " arguments but got " + std::to_string(arguments.size());
//        }
//        return ((ObjectFunction*)callee.as.object)->functionCall(this, arguments);
//    }
}

RuntimeValue Interpreter::evaluateArrayLiteralExpression(ArrayLiteralExpression *expr) {
    std::vector<RuntimeValue> elements;
    disallowGC = true;
    for (auto element: expr->elements) {
        elements.push_back(evaluate(element));
        disallowGC = true; // should be set again in case one of the elements is an array, which would have set it back to false, this just puts it back on true, and the final array will make it false again.
    }
    disallowGC = false;
    return {ValueType::Object, {.object = (Object *) allocateArrayObject(elements)}};
}

RuntimeValue Interpreter::evaluateIndexingExpression(IndexingExpression *expr) {
    RuntimeValue array = evaluate(expr->left);
    RuntimeValue index = evaluate(expr->index);

    if (!IS_OBJ(array) || !IS_ARRAY_OBJ(array)) {
        throw IndexingNonArray(expr, array);
    }
    if (index.type != ValueType::Number) {
        throw WrongTypeError(L"[]", index, expr->index);
    }

    auto elements = GET_ARRAY_OBJ_ELEMENTS(array);
    if (index.as.number < 0 || index.as.number >= elements.size()) {
        throw IndexOutOfBounds(expr->index, index.as.number);
    }

    if (index.as.number != (int) index.as.number) {
        throw NonIntegerIndex(expr->index, index.as.number);
    }

    return elements[(size_t) index.as.number];
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
            if (IS_STRING_OBJ(left) && IS_STRING_OBJ(right)) {
                return GET_STRING_OBJ_VALUE(left) == GET_STRING_OBJ_VALUE(right);
            }
            return left.as.object == right.as.object;
//            throw "EQUALITY NOT YET IMPLEMENTED FOR NON STRING!";
        default:
            return false;
    }
}

ObjectString *Interpreter::allocateStringObject(const std::wstring &value) {
    invokeGarbageCollector();

    auto *obj = new ObjectString();
    obj->obj.type = ObjectType::OBJECT_STRING;
    obj->value = value;
    obj->obj.next = objects;
    objects = (Object *) obj;
    bytesAllocated += sizeof(ObjectString) + sizeof(wchar_t) * value.size();
    return obj;
}

ObjectFunction *Interpreter::allocateFunctionObject(FunctionDeclarationStatement *declaration) {
    invokeGarbageCollector();

    auto *obj = new ObjectFunction(declaration, &environments.top());
    obj->obj.next = objects;
    objects = (Object *) obj;
    bytesAllocated += sizeof(ObjectFunction);
    return obj;
}

ObjectArray *Interpreter::allocateArrayObject(const std::vector<RuntimeValue> &elements) {
    // Arrays don't trigger GC because it could potentially lead to its elements being deleted.

    auto *obj = new ObjectArray();
    obj->obj.type = ObjectType::OBJECT_ARRAY;
    obj->elements = elements;
    obj->obj.next = objects;
    objects = (Object *) obj;
    bytesAllocated += sizeof(ObjectArray) + sizeof(RuntimeValue) * elements.size();
    return obj;
}

void Interpreter::invokeGarbageCollector() {
    if (disallowGC) {
#if DEBUG_LOG_GC == 2
//        std::wcout << L"bk: ---------- gc begin ---------" << std::endl;
//        std::wcout << L"bk: GC disallowed" << std::endl;
//        std::wcout << L"bk: ---------- gc end -----------\n" << std::endl;
#endif
        return;
    }

#ifdef DEBUG_STRESS_GC
    collectGarbage();
    return;
#endif
    if(bytesAllocated > nextGC) {
#if DEBUG_LOG_GC == 1 || DEBUG_LOG_GC == 2
        std::wcout << "\n--------------------------------------------------" << std::endl;
        std::wcout << "Reached current allocation limit of: " << nextGC << " bytes" << std::endl;
        std::wcout << "Bytes allocated before GC: " << bytesAllocated << std::endl;
        clock_t start = clock();
#endif
        collectGarbage();
#if DEBUG_LOG_GC == 1 || DEBUG_LOG_GC == 2
        std::wcout << "Bytes allocated after GC: " << bytesAllocated << std::endl;
        std::wcout << "New limit: " << nextGC << std::endl;
        clock_t end = clock();
        double elapsed = 1000.0 * (end - start) / CLOCKS_PER_SEC;
        std::wcout << "GC took: " << elapsed << " miliseconds" << std::endl;
        std::wcout << "--------------------------------------------------\n" << std::endl;
#endif
    }


}

void Interpreter::collectGarbage() {
#if DEBUG_LOG_GC == 2
    std::wcout << L"bk: ---------- gc begin ---------" << std::endl;
    std::wcout << L"bk: ---------- marking ----------" << std::endl;
//    size_t before = 0;
//    size_t after = 0;
//    for (Object *obj = objects; obj != nullptr; obj = obj->next) {
//        after++;
//    }
//    std::wcout << L"Collected " << before - after << L" objects." << std::endl;
//    std::wcout << L"-- gc end" << std::endl;
#endif

    // traverse objects linked list and delete each one JUST DEBUGGING
//    Object* previous = nullptr;
//    Object* object = objects;
//    while(object != nullptr){
//
//        Object* unreached = object;
//        object = object->next;
//        if(previous != nullptr){
//            previous->next = object;
//        } else {
//            objects = object;
//        }
//        deleteObject(unreached);
//    }

    markRoots();
#if DEBUG_LOG_GC == 2
    std::wcout << L"bk: ---------- tracing ----------" << std::endl;
#endif
    traceReferences();
#if DEBUG_LOG_GC == 2
    std::wcout << L"bk: ---------- sweeping ----------" << std::endl;
#endif
    sweep();

    nextGC = bytesAllocated * 2;

#if DEBUG_LOG_GC == 2
    std::wcout << L"bk: ---------- gc end -----------\n" << std::endl;
#endif
}

void Interpreter::markRoots() {
    std::stack<Environment> envCopy = environments;
    while (!envCopy.empty()) {
        Environment &environment = envCopy.top();
        for (auto &variable: environment.variables) {
            markValue(variable.second.first);
        }
        envCopy.pop();
    }
#if DEBUG_LOG_GC == 2
    std::wcout << L"bk: Marking last returned value:" << std::endl;
#endif
    markValue(returnedValue); // so that the returned value is not collected in case it has not yet been used.

//    for(auto& variable : globals->variables){
//        markValue(variable.second.first);
//    } seems not to be necessary as globals is actually the top environment of the stack that has already been copied and traversed.
}

void Interpreter::markValue(const RuntimeValue &value) {
    if (value.type == ValueType::Object) {
        markObject(value.as.object);
    }
}

void Interpreter::markObject(Object *object) {
    if (object == nullptr) return;
    if (object->isMarked) return;
#if DEBUG_LOG_GC == 2
    std::wcout << L"bk: Marking object  " << object << L", " << getObjectLogString(object) << std::endl;
#endif
    object->isMarked = true;
    grayObjects.push(object);
}

void Interpreter::traceReferences() {
    while (!grayObjects.empty()) {
        Object *object = grayObjects.top();
        grayObjects.pop();
        blackenObject(object);
    }
}

void Interpreter::blackenObject(Object *object) {
#if DEBUG_LOG_GC == 2
    std::wcout << L"bk: Blacken object  " << object << L", " << getObjectLogString(object) << std::endl;
#endif
    switch (object->type) {
        case ObjectType::OBJECT_STRING:
        case ObjectType::OBJECT_CALLABLE: // check if callable is just used for native functions
        case ObjectType::OBJECT_FUNCTION: // function has nothing to have marked, since the functions environment will get pushed onto the stack by executeBlock and hence it will be scanned by markRoots. This includes local variables as well as the function arguments, since they are all part of the function's environment that gets pushed onto the stack.
            break;
        case ObjectType::OBJECT_ARRAY: {
            auto *array = (ObjectArray *) object;
            for (auto &element: array->elements) {
                markValue(element);
            }
            break;
        }
        default:
            throw std::runtime_error("INTERNAL ERROR: Unknown object type in garbage collector");
    }
}

void Interpreter::sweep() {
    Object *previous = nullptr;
    Object *object = objects;
    while (object != nullptr) {
        if (object->isMarked) {
            object->isMarked = false;
            previous = object;
            object = object->next;
        } else {
            Object *unreached = object;
            object = object->next;
            if (previous != nullptr) {
                previous->next = object;
            } else {
                objects = object;
            }
            deleteObject(unreached);
        }
    }
}

void Interpreter::deleteObject(Object *object) {
#if DEBUG_LOG_GC == 2
    std::wcout << L"bk: Deleting object " << object << L", " << getObjectLogString(object) << std::endl;
#endif
    switch (object->type) {
        case ObjectType::OBJECT_STRING:
            bytesAllocated -= sizeof(ObjectString) + sizeof(wchar_t) * ((ObjectString *) object)->value.size();
            delete (ObjectString *) object;
            break;
        case ObjectType::OBJECT_CALLABLE:
            delete (ObjectCallable *) object;
            break;
        case ObjectType::OBJECT_FUNCTION:
            bytesAllocated -= sizeof(ObjectFunction);
            delete (ObjectFunction *) object;
            break;
        case ObjectType::OBJECT_ARRAY:
//            for(auto& element : ((ObjectArray*)object)->elements){
//                if(element.type == ValueType::Object){
//                    deleteObject(element.as.object);
//                }
//            } maybe isn't necessary since if the array itself isn't marked its elements won't be either, so they'll get deleted anyway. This seems to cause double deletion of objects / segfault.
            bytesAllocated -= sizeof(ObjectArray);
            delete (ObjectArray *) object;
            break;
        default:
            throw std::runtime_error("INTERNAL ERROR: Unknown object type in garbage collector");
    }

}

std::wstring Interpreter::getObjectLogString(Object *object) {
    std::wstringstream log;
    log << L"object type: " << std::setw(10) << std::left << getObjectTypeName(object->type) << L"identifier: "
        << getObjectIdentifier(object);
    return log.str();
}

RuntimeError *Interpreter::reallocateError(RuntimeError *error) {

    delete handledError; // In case one was already allocated

    if (dynamic_cast<WrongTypeError *>(error) != nullptr) {
        handledError = new WrongTypeError(*dynamic_cast<WrongTypeError *>(error));
    } else if (dynamic_cast<WrongBinaryOperandTypes *>(error) != nullptr) {
        handledError = new WrongBinaryOperandTypes(*dynamic_cast<WrongBinaryOperandTypes *>(error));
    } else if (dynamic_cast<InvalidCall *>(error) != nullptr) {
        handledError = new InvalidCall(*dynamic_cast<InvalidCall *>(error));
    } else if (dynamic_cast<InvalidArgumentCount *>(error) != nullptr) {
        handledError = new InvalidArgumentCount(*dynamic_cast<InvalidArgumentCount *>(error));
    } else if (dynamic_cast<UndeclaredVariable *>(error) != nullptr) {
        handledError = new UndeclaredVariable(*dynamic_cast<UndeclaredVariable *>(error));
    } else if (dynamic_cast<VariableRedeclaration *>(error) != nullptr) {
        handledError = new VariableRedeclaration(*dynamic_cast<VariableRedeclaration *>(error));
    } else if (dynamic_cast<ConstReassignment *>(error) != nullptr) {
        handledError = new ConstReassignment(*dynamic_cast<ConstReassignment *>(error));
    } else if (dynamic_cast<IndexOutOfBounds *>(error) != nullptr) {
        handledError = new IndexOutOfBounds(*dynamic_cast<IndexOutOfBounds *>(error));
    } else if (dynamic_cast<NonIntegerIndex *>(error) != nullptr) {
        handledError = new NonIntegerIndex(*dynamic_cast<NonIntegerIndex *>(error));
    } else if (dynamic_cast<IndexingNonArray *>(error) != nullptr) {
        handledError = new IndexingNonArray(*dynamic_cast<IndexingNonArray *>(error));
    } else {
        throw std::runtime_error("ERROR REALLOCATION ERROR: Unknown error type");
    }
    return handledError;
}

RuntimeValue Interpreter::lookUpVariable(const VariableExpression *expr) {
    auto distance = locals.find(expr);
    if (distance != locals.end()) {
        return environments.top().getAt(distance->second, expr->name->value);
    } else {
        return globals->get(expr->name);
    }
}