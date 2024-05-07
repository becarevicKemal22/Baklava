//
// Created by kemal on 2/25/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "Object.h"
#include "../TestHelpers.h"

#include "WrongBinaryOperandTypes.h"


//*************************************
// VALID BINARY EXPRESSIONS
//*************************************

TEST_CASE("Computes two number addition", "[interpreter][binary]") {
    std::wstring source = L"5 + 2";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 7);
}

TEST_CASE("Computes two number subtraction", "[interpreter][binary]") {
    std::wstring source = L"5 - 2";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 3);
}

TEST_CASE("Computes two number multiplication", "[interpreter][binary]") {
    std::wstring source = L"5 * 2";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 10);
}

TEST_CASE("Computes two number division", "[interpreter][binary]") {
    std::wstring source = L"5 / 2";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 2.5);
}

TEST_CASE("Computes two string concatenation", "[interpreter][binary]") {
    std::wstring source = L"\"hello\" + \"world\"";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Object);
    REQUIRE(result.as.object->type == ObjectType::OBJECT_STRING);
    auto *objectString = (ObjectString *) result.as.object;
    REQUIRE(objectString->value == L"helloworld");
}

//*************************************
// INVALID BINARY EXPRESSIONS
//*************************************

TEST_CASE("Throws on number + string", "[interpreter][binary]") {
    std::wstring source = L"5 + \"string\"";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on number + boolean", "[interpreter][binary]") {
    std::wstring source = L"5 + tačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on number + null", "[interpreter][binary]") {
    std::wstring source = L"5 + null";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on null + null", "[interpreter][binary]") {
    std::wstring source = L"null + null";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on null + boolean value", "[interpreter][binary]") {
    std::wstring source = L"null + tačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on boolean value + boolean value", "[interpreter][binary]") {
    std::wstring source = L"tačno + netačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on number - string", "[interpreter][binary]") {
    std::wstring source = L"5 - \"string\"";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on number - boolean", "[interpreter][binary]") {
    std::wstring source = L"5 - tačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on number - null", "[interpreter][binary]") {
    std::wstring source = L"5 - null";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on null - null", "[interpreter][binary]") {
    std::wstring source = L"null - null";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on null - boolean value", "[interpreter][binary]") {
    std::wstring source = L"null - tačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on boolean value - boolean value", "[interpreter][binary]") {
    std::wstring source = L"tačno - netačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on number * string", "[interpreter][binary]") {
    std::wstring source = L"5 * \"string\"";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on number * boolean", "[interpreter][binary]") {
    std::wstring source = L"5 * tačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on number * null", "[interpreter][binary]") {
    std::wstring source = L"5 * null";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on null * null", "[interpreter][binary]") {
    std::wstring source = L"null * null";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on null * boolean value", "[interpreter][binary]") {
    std::wstring source = L"null * tačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on boolean value * boolean value", "[interpreter][binary]") {
    std::wstring source = L"tačno * netačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on number / string", "[interpreter][binary]") {

    std::wstring source = L"5 / \"string\"";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on number / boolean", "[interpreter][binary]") {
    std::wstring source = L"5 / tačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on number / null", "[interpreter][binary]") {
    std::wstring source = L"5 / null";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on null / null", "[interpreter][binary]") {
    std::wstring source = L"null / null";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on null / boolean value", "[interpreter][binary]") {
    std::wstring source = L"null / tačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}


TEST_CASE("Throws on boolean value / boolean value", "[interpreter][binary]") {
    std::wstring source = L"tačno / netačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}
