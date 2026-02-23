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
//#define DEBUG_TRACK_EXECUTION
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
#include "GetExpression.h"
#include "SetExpression.h"

#include "BaseFunctions.h"

#include "WrongTypeError.h"
#include "WrongBinaryOperandTypes.h"
#include "GroupingExpression.h"
#include "FunctionDeclarationStatement.h"
#include "InvalidCall.h"
#include "TooManyArguments.h"
#include "TooFewArguments.h"
#include "RuntimeValue.h"
#include "ReturnStatement.h"
#include "Resolver.h"
#include "IndexOutOfBounds.h"
#include "NonIntegerIndex.h"
#include "IndexingNonArray.h"
#include "ConstructorNoNew.h"
#include "ClassNotFound.h"
#include "InvalidPropertyAccess.h"
#include "ObjHasNoAttr.h"

#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <cassert>

#include "ClassDeclarationStatement.h"
#include "ModifyStatement.h"
#include "ThisExpression.h"
#include "WrongTypeToStatement.h"

void Interpreter::defineNativeFunctions() {
    for (auto &nativeFunction: BaseFunctions::getFunctions(this)) {
        ObjectCallable *callable = ((ObjectCallable *) nativeFunction.function.as.object);
        int expectedDefaultArgumentCount = callable->arity - callable->minArity;
        assert(expectedDefaultArgumentCount == callable->defaultArguments.size());
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
        case AstNodeType::ModifyStatement:
            executeModifyStatement(static_cast<ModifyStatement *>(stmt));
            return;
        case AstNodeType::ClassDeclarationStatement:
            executeClassDeclarationStatement(static_cast<ClassDeclarationStatement *>(stmt));
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
            } else if (IS_CLASS_OBJ(value)) {
                os << L"<klasa ";
                os << AS_CLASS_OBJ(value)->name;
                os << L">";
                return;
            } else if (IS_INSTANCE_OBJ(value)) {
                os << "<";
                os << AS_INSTANCE_OBJ(value)->klass->name;
                os << L" instanca>";
                return;
            }
            throw std::runtime_error("PRINT NOT YET IMPLEMENTED FOR THIS OBJECT TYPE!");
        default:
            throw std::runtime_error("UNKNOWN TYPE TO PRINT");
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
    if (stmt->isForLoop) {
        RuntimeValue incrementValue = evaluate(stmt->forIncrement);
        //          auto whileBody = static_cast<BlockStatement*>(stmt->body);
        //          auto incrementExpr = static_cast<ExpressionStatement*>(whileBody->statements.back())->expr;
        //          auto assignExpr = static_cast<AssignmentExpression*>(incrementExpr);
        //          auto binaryExpr = static_cast<BinaryExpression*>(assignExpr->value);
        //          auto incrementValue = evaluate(binaryExpr->right);
        //          if(dynamic_cast<BinaryExpression)

        //        if(incrementValue.type != ValueType::Number){
        //            throw WrongTypeError(L"for loop increment", incrementValue, stmt->forIncrement);
        //        }
        // Provjeriti da li se uopste mora postavljati ovo ako je veci od 0? Valjda bi to po defaultu trebalo biti postavljeno? Mislim da je bitno >= jer ako je 0 onda treba po defaultu da se gleda manje jer je kao beskonacna petlja ali opet korisnik moze rucno postaviti varijablu na neku vrijednost unutar tijela petlje, mada mozda bi htio i da gleda da li je manje.
        // Ali treba ovo vece jednako da bude sto sam vec objasnio u prethodnom tako da mozda bolje ne dirati ne znammmm
        if (incrementValue.as.number >= 0) {
            static_cast<BinaryExpression *>(stmt->condition)->op->type = TokenType::Less;
        } else {
            static_cast<BinaryExpression *>(stmt->condition)->op->type = TokenType::Greater;
        }
    }
    while (isTruthy(evaluate(stmt->condition)) && !isReturning) {
        execute(stmt->body);
    }
}

void Interpreter::executeFunctionDeclarationStatement(FunctionDeclarationStatement *stmt) {
    environments.top().define(stmt->name, {ValueType::Object, {.object = (Object *) allocateFunctionObject(stmt)}},
                              false);
}

void Interpreter::executeClassDeclarationStatement(ClassDeclarationStatement *stmt) {
    environments.top().define(stmt->name, {ValueType::Null}, false);

    std::unordered_map<std::wstring, RuntimeValue> methods;
    for (auto method: stmt->methods) {
        auto func = allocateFunctionObject(method);
        methods.insert({method->name->value, {ValueType::Object, {.object = (Object *) func}}});
    }

    environments.top().assign(
        stmt->name, {ValueType::Object, {.object = (Object *) allocateClassObject(stmt, methods)}});
}

void Interpreter::executeReturnStatement(ReturnStatement *stmt) {
    RuntimeValue value = {ValueType::Null};
    if (stmt->value != nullptr) {
        value = evaluate(stmt->value);
    }
    returnedValue = value;
    isReturning = true;
}

void Interpreter::executeModifyStatement(ModifyStatement *stmt) {
    RuntimeValue currentValue = evaluate(stmt->lvalue);
    RuntimeValue incrementValue = evaluate(stmt->by);

    if (currentValue.type != ValueType::Number) {
        throw WrongTypeToStatement(stmt->keyword, currentValue, stmt->lvalue);
    }

    if (incrementValue.type != ValueType::Number) {
        throw WrongTypeToStatement(stmt->keyword, incrementValue, stmt->by);
    }

    switch (stmt->modificationType) {
        case INCREMENT:
            currentValue.as.number += incrementValue.as.number;
            break;
        case DECREMENT:
            currentValue.as.number -= incrementValue.as.number;
            break;
        case MULTIPLY:
            currentValue.as.number *= incrementValue.as.number;
            break;
        case DIVIDE:
            currentValue.as.number /= incrementValue.as.number;
            break;
    }

    if (stmt->lvalue->type == AstNodeType::VariableExpression) {
        auto expr = static_cast<VariableExpression *>(stmt->lvalue);
        auto distance = locals.find(expr);
        if (distance != locals.end()) {
            environments.top().assignAt(distance->second, expr->name->value, currentValue);
        } else {
            globals->assign(expr->name, currentValue);
        }
    } else {
        // This code is almost identical to the one found in evaluateIndexAssignment, but i dont think its worth extracting to a function or any other solution.
        auto expr = static_cast<IndexingExpression *>(stmt->lvalue);
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

        AS_ARRAY_OBJ(array)->elements[index.as.number].as.number = currentValue.as.number;
    }
}

RuntimeValue Interpreter::evaluate(Expression *expr) {
    switch (expr->type) {
        case AstNodeType::BinaryExpression: {
            disallowGC = true;
            // trenutno nisam siguran da li je moguce da dodje do problema zbog brisanja objekata tokom izvrsavanja nizanih binarnih operacija tipa konkatenacije stringova tako da je najbolje da bude disallowed jer onda nema sanse da ce se izbrisati nesto sto ne treba tokom izvrsavanja binarnih operacija.
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
            return lookUpVariable(static_cast<VariableExpression *>(expr),
                                  static_cast<VariableExpression *>(expr)->name);
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
        case AstNodeType::GetExpression:
            return evaluateGetExpression(static_cast<GetExpression *>(expr));
        case AstNodeType::SetExpression:
            return evaluateSetExpression(static_cast<SetExpression *>(expr));
        case AstNodeType::ThisExpression:
            return evaluateThisExpression(static_cast<ThisExpression *>(expr));
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
                IS_STRING_OBJ(right)) {
                // OVDJE ISPOD TREBA NEKI AS_STR MACRO
                return {
                    ValueType::Object, {
                        .object = (Object *) allocateStringObject(
                            GET_STRING_OBJ_VALUE(left) + GET_STRING_OBJ_VALUE(right))
                    }
                };
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
            throw WrongBinaryOperandTypes(expr->op->value, left, right, expr);
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
            throw WrongBinaryOperandTypes(expr->op->value, left, right, expr);
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
            throw WrongBinaryOperandTypes(expr->op->value, left, right, expr);
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
            throw WrongBinaryOperandTypes(expr->op->value, left, right, expr);
        case TokenType::DoubleEqual:
            return {ValueType::Boolean, {.boolean = isEqual(left, right)}};
        case TokenType::NotEqual:
            return {ValueType::Boolean, {.boolean = !isEqual(left, right)}};
    }
    throw std::runtime_error("Unknown binary operator type!");
}

//
// Moguce ovo bez funkcije samo ubaciti return gore u switch ako se ne bude ovdje vise nista dodavalo
// Nek stoji ovdje funkcija jos ali je sad u switchu samo lookupVariable
//
RuntimeValue Interpreter::evaluateVariableExpression(VariableExpression *expr) {
    return lookUpVariable(expr, expr->name);
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
    // array is modified in place, so that the actual array or its memory location is not changed.
    AS_ARRAY_OBJ(array)->elements[(size_t) index.as.number] = value;

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

// used for class instantiation as well since class inherits from callable
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
    if (!IS_CALLABLE_OBJ(callee) && !IS_FUNCTION_OBJ(callee) && !IS_CLASS_OBJ(callee)) {
        throw InvalidCall(callee, getMostRelevantToken(expr->callee));
    }
    if (expr->isNewPrefixed && !IS_CLASS_OBJ(callee)) {
        throw ClassNotFound(expr->callee);
    }
    if (!expr->isNewPrefixed && IS_CLASS_OBJ(callee)) {
        throw ConstructorNoNew(expr, callee);
    }

    ObjectCallable *callable = AS_CALLABLE_OBJ(
        callee); // this narrows functions to callables but its fine for the first two checks.
    int arity = callable->arity;
    int minArity = callable->minArity;
    if (arguments.size() > arity) {
        throw TooManyArguments(arity, arguments.size(), getMostRelevantToken(expr->callee), expr->paren);
    }
    if (arguments.size() < minArity) {
        throw TooFewArguments(minArity, arguments.size(), getMostRelevantToken(expr->callee), expr->paren);
    }
    // general note on this part: callables are created with pre-evaluated runtime values in the nativeFunctions folder or wherever it is,
    // while functions' default arguments have to be evaluated every time the function is called, since its possible for
    // the user to write code which changes the value of the default parameter from one execution to the next. That's also why the callables' default
    // arguments can be 'pre-evaluated', as I write the code for those and can ensure that the value of default params
    // is never changed.
    if (arguments.size() < arity) {
        if (IS_CALLABLE_OBJ(callee)) {
            for (size_t i = arguments.size(); i < arity; i++) {
                arguments.push_back(callable->defaultArguments[i]);
            }
        } else {
            // depends on the fact that only callables and functions are allowed to pass down to this point.
            ObjectFunction *function = AS_FUNCTION_OBJ(callee);
            disallowGC = true;
            int numOfOptionalAllowedParams = arity - minArity;
            int indexOfFirstMissingParam = numOfOptionalAllowedParams - (arity - arguments.size());
            for (size_t i = indexOfFirstMissingParam; i < numOfOptionalAllowedParams; i++) {
                arguments.push_back(evaluate(function->declaration->defaultParameters[i]));
            }
            disallowGC = false;
        }
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
        disallowGC = true;
        // should be set again in case one of the elements is an array, which would have set it back to false, this just puts it back on true, and the final array will make it false again.
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

RuntimeValue Interpreter::evaluateGetExpression(GetExpression *expr) {
    RuntimeValue instance = evaluate(expr->object);
    if (!IS_OBJ(instance) || !IS_INSTANCE_OBJ(instance)) {
        throw InvalidPropertyAccess(expr->name, instance);
    }

    if (auto val = AS_INSTANCE_OBJ(instance)->fields.find(expr->name->value);
        val != AS_INSTANCE_OBJ(instance)->fields.end()) {
        return val->second;
    }

    if (auto methodIt = AS_INSTANCE_OBJ(instance)->klass->methods.find(expr->name->value);
        methodIt != AS_INSTANCE_OBJ(instance)->klass->methods.end()) {
        auto method = AS_FUNCTION_OBJ(methodIt->second);
        return createFunctionWithBoundThis(method, instance);
    }

    throw ObjHasNoAttr(expr->name, instance);
    // eh fazon znaci treba bacati ovo ali je problem kako struktuirati poruku greske. U pythonu ide 'Obj' object has no attribute 'name'. Eh sad kako to prevesti, da li objekat tipa 'A' ili kako? MIslim onda se to bas opet ne poklapa sa onim da ce se refaktorisati kod kasnije da se koristi jedinstvena funkcija za stringifajanje tipova, a trebala bla bla cekaj ba pa i treba mi kao objekat tipa 'A instanca' nema polje tralala to je okej znaci treba koristit jedinstvenu funkcijui koju ja nemam yippie. Isto tako ne znam da li bi smio staviti kao instanca klase 'x' nema attribut mada to svakako nema smisla a pitanje je hoce li nekad kasnije postojati drugi tipovi koji koriste properties.
    // Takodjer da li ovo treba promijeniti sa attr na nesto drugo jer sad hendlujemo i atribute i metode? Da li su metode atributi?
}

RuntimeValue Interpreter::evaluateSetExpression(SetExpression *expr) {
    RuntimeValue object = evaluate(expr->object);

    if (!IS_OBJ(object) || !IS_INSTANCE_OBJ(object)) {
        throw InvalidPropertyAccess(expr->name, object);
    }
    RuntimeValue value = evaluate(expr->value);
    ((ObjectInstance *) object.as.object)->fields[expr->name->value] = value;
    // check if this needs to be split into multiple variables in order to do some checks?
    // Actually it def will need to be split when slots are implemented so that the property name can be checked for existence
    // ovo za split sam mislio prije nego sto sam izdvojio value da ga mogu vratiti. Elem svakako ce za slots trebat i field provjeravat u posebnoj varijabli ili bez sa castom ugl provjeriti
    return value;
}

RuntimeValue Interpreter::evaluateThisExpression(ThisExpression *expr) {
    return lookUpVariable(expr, expr->token);
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

ObjectFunction *Interpreter::allocateFunctionObject(FunctionDeclarationStatement *declaration, Environment *env) {
    invokeGarbageCollector();

    auto *obj = new ObjectFunction(declaration, env == nullptr ? &environments.top() : env);
    obj->obj.next = objects;
    objects = (Object *) obj;
    bytesAllocated += sizeof(ObjectFunction);
    return obj;
}

RuntimeValue Interpreter::createFunctionWithBoundThis(ObjectFunction *method, RuntimeValue instance) {
    Environment env(&method->closure);
    env.defineAndBindThis(instance);
    return {ValueType::Object, {.object = (Object *) allocateFunctionObject(method->declaration, &env)}};
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

ObjectClass *Interpreter::allocateClassObject(ClassDeclarationStatement *declaration,
                                              std::unordered_map<std::wstring, RuntimeValue> &methods) {
    invokeGarbageCollector();

    auto *obj = new ObjectClass(declaration, methods);
    obj->call = [obj, this](Interpreter *interpreter, const std::vector<RuntimeValue> &arguments) {
        // interpreter->invokeGarbageCollector(); // I HAVE NO CLUE WHETHER THIS CAN MESS SOMETHING UP. ACTUALLY IT IS COMMENTED BECAUSE ALLOCATE INSTANCE OBJECT CALLS IT ITSELF????

        RuntimeValue instance = {ValueType::Object, {.object = (Object *) interpreter->allocateInstanceObject(obj)}};
        if (auto konstruktor = obj->konstruktor) {
            AS_FUNCTION_OBJ(createFunctionWithBoundThis(konstruktor, instance))->call(this, arguments);
        }
        return instance;
    };
    obj->obj.next = objects;
    objects = (Object *) obj;
    bytesAllocated += sizeof(ObjectClass);
    // obj->methods = std::move(methods); prije je ovako bilo sad prosljedjujem u konstruktor. Ne bi trebalo praviti razliku?
    return obj;
}

ObjectInstance *Interpreter::allocateInstanceObject(ObjectClass *klass) {
    invokeGarbageCollector(); // THIS OK???? IDK IF IT CAN DELETE SOMETHING IN THE MEANTIME

    auto *obj = new ObjectInstance();
    obj->obj.type = ObjectType::OBJECT_INSTANCE;
    obj->klass = klass;
    obj->obj.next = objects;
    objects = (Object *) obj;
    bytesAllocated += sizeof(ObjectInstance);
    return obj;
}

void Interpreter::invokeGarbageCollector() {
    return;
    if (disallowGC) {
#if DEBUG_LOG_GC == 2
        std::wcout << L"bk: ---------- gc begin ---------" << std::endl;
        std::wcout << L"bk: GC disallowed" << std::endl;
        std::wcout << L"bk: ---------- gc end -----------\n" << std::endl;
#endif
        return;
    }

#ifdef DEBUG_STRESS_GC
    collectGarbage();
    return;
#endif
    if (bytesAllocated > nextGC) {
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

    //    nextGC = bytesAllocated * 2;

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
        case ObjectType::OBJECT_FUNCTION:
            // function has nothing to have marked, since the functions environment will get pushed onto the stack by executeBlock and hence it will be scanned by markRoots. This includes local variables as well as the function arguments, since they are all part of the function's environment that gets pushed onto the stack.
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
    } else if (dynamic_cast<TooManyArguments *>(error) != nullptr) {
        handledError = new TooManyArguments(*dynamic_cast<TooManyArguments *>(error));
    } else if (dynamic_cast<TooFewArguments *>(error) != nullptr) {
        handledError = new TooFewArguments(*dynamic_cast<TooFewArguments *>(error));
    } else if (dynamic_cast<UndeclaredIdentifier *>(error) != nullptr) {
        handledError = new UndeclaredIdentifier(*dynamic_cast<UndeclaredIdentifier *>(error));
    } else if (dynamic_cast<IdentifierRedeclaration *>(error) != nullptr) {
        handledError = new IdentifierRedeclaration(*dynamic_cast<IdentifierRedeclaration *>(error));
    } else if (dynamic_cast<ConstReassignment *>(error) != nullptr) {
        handledError = new ConstReassignment(*dynamic_cast<ConstReassignment *>(error));
    } else if (dynamic_cast<IndexOutOfBounds *>(error) != nullptr) {
        handledError = new IndexOutOfBounds(*dynamic_cast<IndexOutOfBounds *>(error));
    } else if (dynamic_cast<NonIntegerIndex *>(error) != nullptr) {
        handledError = new NonIntegerIndex(*dynamic_cast<NonIntegerIndex *>(error));
    } else if (dynamic_cast<IndexingNonArray *>(error) != nullptr) {
        handledError = new IndexingNonArray(*dynamic_cast<IndexingNonArray *>(error));
    } else if (dynamic_cast<WrongTypeToStatement *>(error) != nullptr) {
        handledError = new WrongTypeToStatement(*dynamic_cast<WrongTypeToStatement *>(error));
    } else if (dynamic_cast<ConstructorNoNew *>(error) != nullptr) {
        handledError = new ConstructorNoNew(*dynamic_cast<ConstructorNoNew *>(error));
    } else if (dynamic_cast<ClassNotFound *>(error) != nullptr) {
        handledError = new ClassNotFound(*dynamic_cast<ClassNotFound *>(error));
    } else if (dynamic_cast<InvalidPropertyAccess *>(error) != nullptr) {
        handledError = new InvalidPropertyAccess(*dynamic_cast<InvalidPropertyAccess *>(error));
    } else if (dynamic_cast<ObjHasNoAttr *>(error) != nullptr) {
        handledError = new ObjHasNoAttr(*dynamic_cast<ObjHasNoAttr *>(error));
    } else {
        throw std::runtime_error("ERROR REALLOCATION ERROR: Unknown error type");
    }
    return handledError;
}

RuntimeValue Interpreter::lookUpVariable(const Expression *expr, TokenPtr name) {
    auto distance = locals.find(expr);
    if (distance != locals.end()) {
        return environments.top().getAt(distance->second, name->value);
    }
    return globals->get(name);
}
