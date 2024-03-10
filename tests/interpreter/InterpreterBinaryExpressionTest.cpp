//
// Created by kemal on 2/25/2024.
//

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"


//*************************************
// VALID BINARY EXPRESSIONS
//*************************************

TEST_CASE("Computes two number addition", "[interpreter][binary]"){
    std::wstring source = L"5 + 2";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 7);
}

TEST_CASE("Computes two number subtraction", "[interpreter][binary]"){
    std::wstring source = L"5 - 2";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 3);
}

TEST_CASE("Computes two number multiplication", "[interpreter][binary]"){
    std::wstring source = L"5 * 2";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 10);
}

TEST_CASE("Computes two number division", "[interpreter][binary]"){
    std::wstring source = L"5 / 2";
    Interpreter interpreter;

    RuntimeValue result = interpreter.evaluate(parseSingleExpression(source));
    REQUIRE(result.type == ValueType::Number);
    REQUIRE(result.as.number == 2.5);
}

TEST_CASE("Computes two string concatenation", "[interpreter][binary]"){
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

TEST_CASE("Reports error on number + string", "[interpreter][binary]"){
    std::wstring source = L"5 + \"string\"";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on number + boolean", "[interpreter][binary]"){
    std::wstring source = L"5 + istina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on number + nula", "[interpreter][binary]"){
    std::wstring source = L"5 + nula";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on nula + nula", "[interpreter][binary]"){
    std::wstring source = L"nula + nula";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on nula + boolean value", "[interpreter][binary]"){
    std::wstring source = L"nula + istina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on boolean value + boolean value", "[interpreter][binary]"){
    std::wstring source = L"istina + neistina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on number - string", "[interpreter][binary]"){
    std::wstring source = L"5 - \"string\"";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on number - boolean", "[interpreter][binary]"){
    std::wstring source = L"5 - istina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on number - nula", "[interpreter][binary]"){
    std::wstring source = L"5 - nula";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on nula - nula", "[interpreter][binary]"){
    std::wstring source = L"nula - nula";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on nula - boolean value", "[interpreter][binary]"){
    std::wstring source = L"nula - istina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on boolean value - boolean value", "[interpreter][binary]"){
    std::wstring source = L"istina - neistina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on number * string", "[interpreter][binary]"){
    std::wstring source = L"5 * \"string\"";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on number * boolean", "[interpreter][binary]"){
    std::wstring source = L"5 * istina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on number * nula", "[interpreter][binary]"){
    std::wstring source = L"5 * nula";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on nula * nula", "[interpreter][binary]"){
    std::wstring source = L"nula * nula";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on nula * boolean value", "[interpreter][binary]"){
    std::wstring source = L"nula * istina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on boolean value * boolean value", "[interpreter][binary]"){
    std::wstring source = L"istina * neistina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on number / string", "[interpreter][binary]"){

    std::wstring source = L"5 / \"string\"";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on number / boolean", "[interpreter][binary]"){
    std::wstring source = L"5 / istina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on number / nula", "[interpreter][binary]"){
    std::wstring source = L"5 / nula";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on nula / nula", "[interpreter][binary]"){
    std::wstring source = L"nula / nula";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}

TEST_CASE("Reports error on nula / boolean value", "[interpreter][binary]"){
    std::wstring source = L"nula / istina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}


TEST_CASE("Reports error on boolean value / boolean value", "[interpreter][binary]"){
    std::wstring source = L"istina / neistina";
    Interpreter interpreter;

    RuntimeValue result;

    REQUIRE_NOTHROW(result = interpreter.evaluate(parseSingleExpression(source)));
    REQUIRE(interpreter.hadError);
}
