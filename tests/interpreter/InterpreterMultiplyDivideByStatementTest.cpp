//
// Created by kemal on 26-Feb-25.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "WrongTypeToStatement.h"
#include "../TestHelpers.h"

TEST_CASE("Executes basic multiply by statement", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = 2; pomnozi a sa 2; ispisi a;";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 4);
}

TEST_CASE("Executes basic divide by statement", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = 5; podijeli a sa 2.5; ispisi a;";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}

TEST_CASE("Executes multiply statement with function return value", "[interpreter][modifyStatement]") {
    std::wstring source = L"funkcija f(){vrati 5;}"
            "var a = 5; pomnozi a sa f(); ispisi a;";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 25);
}

TEST_CASE("Executes multiply statement with arrays", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = [1, 2, 3]; "
                          "var b = [2, 3, 4]; "
                          "za svako j od 0 do 3 pomnozi a[j] sa b[j]; "
                          "ispiši a;";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].type == ValueType::Object);
    REQUIRE(GET_ARRAY_OBJ_ELEMENTS(interpreter.printHistory[0]).size() == 3);
    REQUIRE(GET_ARRAY_OBJ_ELEMENTS(interpreter.printHistory[0])[0].as.number == 2);
    REQUIRE(GET_ARRAY_OBJ_ELEMENTS(interpreter.printHistory[0])[1].as.number == 6);
    REQUIRE(GET_ARRAY_OBJ_ELEMENTS(interpreter.printHistory[0])[2].as.number == 12);
}

TEST_CASE("Reports error when trying to multiply non number", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = \"string\"; pomnozi a sa 2;";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source, &interpreter).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<WrongTypeToStatement>(&interpreter);
}

TEST_CASE("Reports when trying to multiply by non number", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = 5; pomnozi a sa \"string\";";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source, &interpreter).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<WrongTypeToStatement>(&interpreter);
}

TEST_CASE("Reports error when indexing non array", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = 5; pomnozi a[0] sa 2;";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source, &interpreter).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<IndexingNonArray>(&interpreter);
}

TEST_CASE("Reports error when using non-numeric index", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = [1, 2, 3]; pomnozi a[\"string\"] sa 2;";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source, &interpreter).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<WrongTypeError>(&interpreter);
}

TEST_CASE("Reports error when indexing out of bounds", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = [1, 2, 3]; pomnozi a[3] sa 2;";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source, &interpreter).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<IndexOutOfBounds>(&interpreter);
}

TEST_CASE("Reports error when using non-integer index", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = [1, 2, 3]; pomnozi a[1.5] sa 2;";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source, &interpreter).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<NonIntegerIndex>(&interpreter);
}