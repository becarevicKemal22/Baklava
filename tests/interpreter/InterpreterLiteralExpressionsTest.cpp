//
// Created by kemal on 2/22/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "Object.h"
#include "../TestHelpers.h"

TEST_CASE("Interprets true boolean literal", "[interpreter][literal]") {
    std::wstring source = L"tačno";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Interprets false boolean literal", "[interpreter][literal]") {
    std::wstring source = L"netačno";
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
    std::wstring source = L"null";
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

TEST_CASE("Interprets empty string literal", "[interpreter][literal]") {
    std::wstring source = L"\"\"";
    Interpreter interpreter;
    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Object);
    REQUIRE(result.as.object->type == ObjectType::OBJECT_STRING);
    auto *objectString = (ObjectString *) result.as.object;
    REQUIRE(objectString->value == L"");
}

TEST_CASE("Interprets empty array literal", "[interpreter][literal]") {
    std::wstring source = L"[]";
    Interpreter interpreter;
    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(IS_OBJ(result));
    REQUIRE(IS_ARRAY_OBJ(result));
    auto *objectArray = AS_ARRAY_OBJ(result);
    REQUIRE(objectArray->elements.size() == 0);
}

TEST_CASE("Interprets array literal with elements", "[interpreter][literal]") {
    std::wstring source = L"[1, 2, 3]";
    Interpreter interpreter;
    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(IS_OBJ(result));
    REQUIRE(IS_ARRAY_OBJ(result));
    auto *objectArray = AS_ARRAY_OBJ(result);
    REQUIRE(objectArray->elements.size() == 3);
    REQUIRE(objectArray->elements[0].as.number == 1);
    REQUIRE(objectArray->elements[1].as.number == 2);
    REQUIRE(objectArray->elements[2].as.number == 3);
}

TEST_CASE("Interprets array literal with mixed-type elements", "[interpreter][literal]") {
    std::wstring source = L"[1, \"string\", null]";
    Interpreter interpreter;
    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(IS_OBJ(result));
    REQUIRE(IS_ARRAY_OBJ(result));
    auto *objectArray = AS_ARRAY_OBJ(result);
    REQUIRE(objectArray->elements.size() == 3);
    REQUIRE(objectArray->elements[0].as.number == 1);
    REQUIRE(objectArray->elements[1].as.object->type == ObjectType::OBJECT_STRING);
    REQUIRE(objectArray->elements[2].type == ValueType::Null);
}
