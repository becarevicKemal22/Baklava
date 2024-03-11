//
// Created by kemal on 3/11/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"

TEST_CASE("Evaluates logical and", "[interpreter][binary][logical]"){
    std::wstring source = L"istina && istina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Evaluates logical or", "[interpreter][binary][logical]"){
    std::wstring source = L"istina || neistina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Evaluates logical and with short-circuiting", "[interpreter][binary][logical]"){
    std::wstring source = L"neistina && istina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == false);
}

TEST_CASE("Evaluates logical or with short-circuiting", "[interpreter][binary][logical]"){
    std::wstring source = L"istina || neistina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Logical or with truthy and falsy values", "[interpreter][binary][logical]"){
    std::wstring source = L"1 || 0";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 1);
}

TEST_CASE("Logical and with truthy and falsy values", "[interpreter][binary][logical]"){
    std::wstring source = L"1 && 0";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 0);
}

TEST_CASE("Logical or with string and null", "[interpreter][binary][logical]"){
    std::wstring source = L"\"string\" || nula";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Object);
    REQUIRE(result.as.object->type == ObjectType::OBJECT_STRING);
    auto *objectString = (ObjectString *) result.as.object;
    REQUIRE(objectString->value == L"string");
}

TEST_CASE("Logical and with string and null", "[interpreter][binary][logical]"){
    std::wstring source = L"\"string\" && nula";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Null);
}

TEST_CASE("Logical or with string and boolean", "[interpreter][binary][logical]"){
    std::wstring source = L"\"string\" || istina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Object);
    REQUIRE(result.as.object->type == ObjectType::OBJECT_STRING);
    auto *objectString = (ObjectString *) result.as.object;
    REQUIRE(objectString->value == L"string");
}

TEST_CASE("Logical and with string and boolean", "[interpreter][binary][logical]"){
    std::wstring source = L"\"string\" && istina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Logical or with number and string", "[interpreter][binary][logical]"){
    std::wstring source = L"5 || \"string\"";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 5);
}

TEST_CASE("Logical and with number and string", "[interpreter][binary][logical]"){
    std::wstring source = L"5 && \"string\"";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Object);
    REQUIRE(result.as.object->type == ObjectType::OBJECT_STRING);
    auto *objectString = (ObjectString *) result.as.object;
    REQUIRE(objectString->value == L"string");
}

TEST_CASE("Logical or with number and boolean", "[interpreter][binary][logical]"){
    std::wstring source = L"5 || istina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 5);
}

TEST_CASE("Logical and with number and boolean", "[interpreter][binary][logical]"){
    std::wstring source = L"5 && istina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Logical or with number and null", "[interpreter][binary][logical]"){
    std::wstring source = L"5 || nula";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 5);
}

TEST_CASE("Logical and with number and null", "[interpreter][binary][logical]"){
    std::wstring source = L"5 && nula";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Null);
}
