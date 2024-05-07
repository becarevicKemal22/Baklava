//
// Created by kemal on 3/11/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"

TEST_CASE("Computes less than comparison", "[interpreter][binary][comparison]"){
    std::wstring source = L"5 < 2";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == false);
}

TEST_CASE("Computes less than or equal comparison", "[interpreter][binary][comparison]"){
    std::wstring source = L"5 <= 5";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Computes greater than comparison", "[interpreter][binary][comparison]"){
    std::wstring source = L"5 > 2";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Computes greater than or equal comparison", "[interpreter][binary][comparison]"){
    std::wstring source = L"5 >= 5";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Computes equal comparison", "[interpreter][binary][comparison]"){
    std::wstring source = L"5 == 5";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Computes not equal comparison", "[interpreter][binary][comparison]"){
    std::wstring source = L"5 != 5";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == false);
}

TEST_CASE("Throws on comparison with different types", "[interpreter][binary][comparison]"){
    std::wstring source = L"5 < \"string\"";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on comparison with null", "[interpreter][binary][comparison]"){
    std::wstring source = L"5 < null";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}

TEST_CASE("Throws on comparison with boolean", "[interpreter][binary][comparison]"){
    std::wstring source = L"5 < tačno";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongBinaryOperandTypes);
}
