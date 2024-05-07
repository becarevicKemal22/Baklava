//
// Created by kemal on 3/12/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"

TEST_CASE("Interprets simple grouping expression", "[interpreter][grouping]") {
    std::wstring source = L"(tačno)";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Interprets simple grouping expression with binary expression", "[interpreter][grouping]") {
    std::wstring source = L"(1 + 2)";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 3);
}

TEST_CASE("Interprets nested grouping expression", "[interpreter][grouping]") {
    std::wstring source = L"(1 + (2 * 3))";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 7);
}

TEST_CASE("Interprets part of expression as grouping", "[interpreter][grouping]") {
    std::wstring source = L"1 + (2 * 3) + 4";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 11);
}

TEST_CASE("Interprets part of expression as grouping with nested grouping", "[interpreter][grouping]") {
    std::wstring source = L"1 + (2 * (3 + 4)) + 5";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 20);
}

TEST_CASE("Interprets part of expression as grouping with nested grouping and unary", "[interpreter][grouping]") {
    std::wstring source = L"1 + (2 * -(3 + 4)) + 5";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == -8);
}
