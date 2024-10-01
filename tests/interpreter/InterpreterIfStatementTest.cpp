//
// Created by kemal on 3/16/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"

TEST_CASE("Executes basic if statement", "[interpreter][controlFlow]") {
    std::wstring source = L"ako tačno onda { ispiši 1; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 1);
}

TEST_CASE("Executes if-else statement", "[interpreter][controlFlow]") {
    std::wstring source = L"ako netačno onda { ispiši 1; } inače { ispiši 2; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}

TEST_CASE("Executes if-else if statement", "[interpreter][controlFlow]") {
    std::wstring source = L"ako netačno onda { ispiši 1; } inače ako tačno onda { ispiši 2; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}

TEST_CASE("Executes shorthand if statement with 'je'", "[interpreter][controlFlow]") {
    std::wstring source = L"var a = tacno;"
                          "var b = netacno; "
                          "ako je a tačno onda { ispiši 1; }"
                          "ako je b tacno onda { ispiši 2; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 1);
}

TEST_CASE("Executes shorthand if statement with 'je' 2", "[interpreter][controlFlow]") {
    std::wstring source = L"var a = netacno; "
                          "ako je a tačno onda { ispiši 1; } inače { ispiši 2; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}

TEST_CASE("Executes shorthand if statement with 'je' 3", "[interpreter][controlFlow]") {
    std::wstring source = L"funkcija f(){vrati null;}"
                          "ako je f() tačno onda { ispiši 1; }"
                          "ako je f() null onda { ispisi 2; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}

TEST_CASE("Supports old way of writing if statements", "[interpreter][controlFlow]") {
    std::wstring source = L"var a = 5;"
                          "var b = 10; "
                          "ako (a > b) { ispiši 1; }"
                          "inace ako (b > a){ ispiši 2; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}