//
// Created by kemal on 9/15/2024.
//

#include "BaseFunctions.h"

std::vector<NativeFunction> BaseFunctions::getFunctions(Interpreter* _interpreter){
    std::vector<NativeFunction> functions;

    RuntimeValue clockFunction;
    clockFunction.type = ValueType::Object;
    auto *clockCallable = new ObjectCallable();
    clockCallable->obj.type = ObjectType::OBJECT_CALLABLE;
    clockCallable->arity = 0;
    clockCallable->call = [](Interpreter *interpreter, const std::vector<RuntimeValue> &arguments) -> RuntimeValue {
        return {ValueType::Number, {.number = (double) clock() / CLOCKS_PER_SEC}};
    };

    clockFunction.as.object = (Object *) clockCallable;

    auto clockToken = generateIdentifierToken(L"sat");

    functions.push_back({clockToken, clockFunction});

    RuntimeValue emptyFunction;
    emptyFunction.type = ValueType::Object;
    ObjectCallable *emptyCallable = new ObjectCallable();
    emptyCallable->obj.type = ObjectType::OBJECT_CALLABLE;
    emptyCallable->arity = 0;
    emptyCallable->call = [](Interpreter *interpreter, const std::vector<RuntimeValue> &arguments) -> RuntimeValue {
        return {ValueType::Null};
    };

    emptyFunction.as.object = (Object *) emptyCallable;

    auto emptyToken = generateIdentifierToken(L"mockFunkcija");

    functions.push_back({emptyToken, emptyFunction});

    RuntimeValue inputFunction;
    inputFunction.type = ValueType::Object;
    ObjectCallable *inputCallable = new ObjectCallable();
    inputCallable->obj.type = ObjectType::OBJECT_CALLABLE;
    inputCallable->arity = 1;
    inputCallable->call = [_interpreter](Interpreter *interpreter, const std::vector<RuntimeValue> &arguments) -> RuntimeValue {
        RuntimeValue message = arguments[0];
        if (message.type != ValueType::Object) {
            std::wcout << "Unos: ";
        } else if (IS_OBJ(message) && !IS_STRING_OBJ(message)) {
            std::wcout << "Unos: ";
        } else std::wcout << GET_STRING_OBJ_VALUE(message);
        std::wstring input;
        std::wcin >> input;
        return {ValueType::Object, {.object = (Object *) _interpreter->allocateStringObject(input)}};
    };

    inputFunction.as.object = (Object *) inputCallable;

    auto inputToken = generateIdentifierToken(L"unos");

    functions.push_back({inputToken, inputFunction});

    RuntimeValue numConversionFunction;
    numConversionFunction.type = ValueType::Object;
    ObjectCallable *numConversionCallable = new ObjectCallable();
    numConversionCallable->obj.type = ObjectType::OBJECT_CALLABLE;
    numConversionCallable->arity = 1;
    numConversionCallable->call = [_interpreter](Interpreter *interpreter,
                                         const std::vector<RuntimeValue> &arguments) -> RuntimeValue {
        RuntimeValue value = arguments[0];
        if (value.type == ValueType::Number) {
            return value;
        }
        if (value.type == ValueType::Object && IS_STRING_OBJ(value)) {
            try {
                return {ValueType::Number, {.number = std::stod(GET_STRING_OBJ_VALUE(value))}};
            } catch (std::invalid_argument &e) {
                throw WrongTypeError(L"konverzija u broj", value, nullptr);
            }
        }
        if (value.type == ValueType::Null || value.type == ValueType::Boolean) {
            return {ValueType::Number, {.number = 0}};
        }
        if (value.type == ValueType::Boolean) {
            return {ValueType::Number, {.number = value.as.boolean ? 1. : 0.}};
        }
        throw WrongTypeError(L"konverzija u broj", value, nullptr);
    };

    numConversionFunction.as.object = (Object *) numConversionCallable;

    auto numConversionToken = generateIdentifierToken(L"broj");

    functions.push_back({numConversionToken, numConversionFunction});

    return functions;
}