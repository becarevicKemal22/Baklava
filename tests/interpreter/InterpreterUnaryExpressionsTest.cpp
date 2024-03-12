//
// Created by kemal on 2/22/2024.
//

#include "catch2/catch_test_macros.hpp"

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"
#include "WrongTypeError.h"


TEST_CASE("Applies logical not to true boolean value", "[interpreter][unary]"){
    std::wstring source = L"!istina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == false);
}

TEST_CASE("Applies logical not to false boolean value", "[interpreter][unary]"){
    std::wstring source = L"!neistina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Applies logical not to falsy value", "[interpreter][unary]"){
    std::wstring source = L"!0";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Applies logical not to truthy value", "[interpreter][unary]"){
    std::wstring source = L"!1";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == false);
}

TEST_CASE("Applies logical not to null", "[interpreter][unary]"){
    std::wstring source = L"!nula";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == true);
}

TEST_CASE("Applies chained logical not", "[interpreter][unary]"){
    std::wstring source = L"!!neistina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == false);
}

TEST_CASE("Negates numeric value", "[interpreter][unary]"){
    std::wstring source = L"-5";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == -5);
}

TEST_CASE("Negates chained numeric value", "[interpreter][unary]"){
    std::wstring source = L"--5";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 5);
}

TEST_CASE("Negates null value", "[interpreter][unary]"){
    std::wstring source = L"-nula";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == -0);
}

TEST_CASE("Throws on boolean value negation", "[interpreter][unary]"){
    std::wstring source = L"-istina";
    Interpreter interpreter;
    RuntimeValue result;
    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongTypeError);
}

TEST_CASE("Throws on false boolean value negation", "[interpreter][unary]"){
    std::wstring source = L"-neistina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongTypeError);
}

TEST_CASE("Throws on string value negation", "[interpreter][unary]"){
    std::wstring source = L"-\"string\"";
    Interpreter interpreter;

    RuntimeValue result;
    REQUIRE_THROWS_AS(result = interpreter.evaluate(parseSingleExpression(source)), WrongTypeError);
}

TEST_CASE("Returns false on logical not of string value", "[interpreter][unary]"){
    std::wstring source = L"!\"string\"";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Boolean);
    REQUIRE(result.as.boolean == false);
}
