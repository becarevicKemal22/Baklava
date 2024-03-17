//
// Created by kemal on 3/16/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"

TEST_CASE("Executes while loop", "[interpreter][controlFlow]") {
    std::wstring source = L"var x = 0; dok (x < 1) { ispisi x; x = x + 1; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 0);
}

TEST_CASE("Does not execute on initial false condition", "[interpreter][controlFlow]") {
    std::wstring source = L"dok (neistina) { ispisi 1; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 0);
}

TEST_CASE("Executes while loop without block", "[interpreter][controlFlow]") {
    std::wstring source = L"var x = 0; dok (x < 10) x = x + 1; ispisi x;";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 10);
}

TEST_CASE("Executes nested while loops", "[interpreter][controlFlow]") {
    std::wstring source = L"var x = 0; dok (x < 2) { var y = 0; dok (y < 2) { ispisi y; y = y + 1; } x = x + 1; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 4);
    REQUIRE(interpreter.printHistory[0].as.number == 0);
    REQUIRE(interpreter.printHistory[1].as.number == 1);
    REQUIRE(interpreter.printHistory[2].as.number == 0);
    REQUIRE(interpreter.printHistory[3].as.number == 1);
}
