//
// Created by kemal on 2/22/2024.
//

#include "catch2/catch_test_macros.hpp"

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"

#include "Lexer.h"
#include "Parser.h"

TEST_CASE("Applies logical not to true boolean value", "[interpreter][unary]"){
    std::wstring source = L"!istina";
    Interpreter interpreter;

//    Lexer lexer(source);
//    lexer.tokenize();
//    Parser parser(lexer.tokens);
//    auto program = parser.parse();


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

TEST_CASE("Negates true boolean value", "[interpreter][unary]"){
    std::wstring source = L"-istina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == -1);
}

TEST_CASE("Negates false boolean value", "[interpreter][unary]"){
    std::wstring source = L"-neistina";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == -0);
}
