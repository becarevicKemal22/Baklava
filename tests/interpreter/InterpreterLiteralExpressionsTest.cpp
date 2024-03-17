//
// Created by kemal on 2/22/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "Object.h"
#include "../TestHelpers.h"

TEST_CASE("Interprets true boolean literal", "[interpreter][literal]") {
    std::wstring source = L"istina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Interprets false boolean literal", "[interpreter][literal]") {
    std::wstring source = L"neistina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == false);
}

TEST_CASE("Interprets numeric literal", "[interpreter][literal]") {
    std::wstring source = L"24.2";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 24.2);
}

TEST_CASE("Interprets null literal", "[interpreter][literal]") {
    std::wstring source = L"nula";
    Interpreter interpreter;
    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Null);
}

TEST_CASE("Interprets string literal", "[interpreter][literal]") {
    std::wstring source = L"\"Hello, world!\"";
    Interpreter interpreter;
    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Object);
    REQUIRE(result.as.object->type == ObjectType::OBJECT_STRING);
    auto *objectString = (ObjectString *) result.as.object;
    REQUIRE(objectString->value == L"Hello, world!");
}
