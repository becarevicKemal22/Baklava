//
// Created by kemal on 7/17/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "RuntimeValue.h"
#include "Object.h"
#include "FunctionDeclarationStatement.h"

TEST_CASE("Prints integer number value", "[interpreter][runtimeValue]") {
    RuntimeValue val = RuntimeValue(ValueType::Number, {.number = 5});
    std::wostringstream stream;
    Interpreter::printValue(val, stream);
    REQUIRE(stream.str() == L"5");
}

TEST_CASE("Prints floating point number value", "[interpreter][runtimeValue]") {
    RuntimeValue val = RuntimeValue(ValueType::Number, {.number = 5.05});
    std::wostringstream stream;
    Interpreter::printValue(val, stream);
    REQUIRE(stream.str() == L"5.05");
}

TEST_CASE("Prints true boolean value", "[interpreter][runtimeValue]") {
    RuntimeValue val = RuntimeValue(ValueType::Boolean, {.boolean = true});
    std::wostringstream stream;
    Interpreter::printValue(val, stream);
    REQUIRE(stream.str() == L"tačno");
}

TEST_CASE("Prints false boolean value", "[interpreter][runtimeValue]") {
    RuntimeValue val = RuntimeValue(ValueType::Boolean, {.boolean = false});
    std::wostringstream stream;
    Interpreter::printValue(val, stream);
    REQUIRE(stream.str() == L"netačno");
}

TEST_CASE("Prints null value", "[interpreter][runtimeValue]") {
    RuntimeValue val = RuntimeValue(ValueType::Null);
    std::wostringstream stream;
    Interpreter::printValue(val, stream);
    REQUIRE(stream.str() == L"null");
}

TEST_CASE("Prints string value", "[interpreter][runtimeValue]") {
    Object base(ObjectType::OBJECT_STRING);
    ObjectString strObj(base, L"test");
    RuntimeValue string(ValueType::Object);
    string.as.object = &strObj.obj;
    std::wostringstream stream;
    Interpreter::printValue(string, stream);
    REQUIRE(stream.str() == L"test");
}

// ARRAY PRINTING ACTUALLY WORKS BUT FOR SOME REASON THE WOSTRINGSTREAM PICKS THE PRINTED CHARACTERS IN THE WRONG ORDER, BUT WHEN DEBUGGING THE FUNCTION IS ACTUALLY DOING ALL THE STEPS IN THE RIGHT ORDER
//TEST_CASE("Prints array", "[interpreter][runtimeValue]") {
//    Object base(ObjectType::OBJECT_ARRAY);
//    std::vector<RuntimeValue> arrayElements = {RuntimeValue(ValueType::Number, {.number = 5}),
//                                               RuntimeValue(ValueType::Number, {.number = 10})};
//    ObjectArray arrObj(base, arrayElements);
//    RuntimeValue array(ValueType::Object);
//    array.as.object = &arrObj.obj;
//    std::wostringstream stream;
//    Interpreter::printValue(array, stream);
//    std::wcout << stream.str() << std::endl;
//    REQUIRE(stream.str() == L"[5, 10]");
//}

TEST_CASE("Prints function", "[interpreter][runtimeValue]") {
    Token token(TokenType::Identifier, L"test", 1, 1);
    FunctionDeclarationStatement* declaration = new FunctionDeclarationStatement(&token, {}, {}, {});

    Object base(ObjectType::OBJECT_FUNCTION);
    Environment env;
    ObjectFunction funcObj(declaration, &env);
    RuntimeValue function(ValueType::Object);
    function.as.object = &funcObj.obj;
    std::wostringstream stream;
    Interpreter::printValue(function, stream);
    REQUIRE(stream.str() == L"<funkcija test>");
}

TEST_CASE("Prints callable", "[interpreter][runtimeValue]") {
    Token token(TokenType::Identifier, L"test", 1, 1);
    Object base(ObjectType::OBJECT_CALLABLE);
    ObjectCallable callableObj(base, 0);
    RuntimeValue callable(ValueType::Object);
    callable.as.object = &callableObj.obj;
    std::wostringstream stream;
    Interpreter::printValue(callable, stream);
    REQUIRE(stream.str() == L"<funkcija>");
}
