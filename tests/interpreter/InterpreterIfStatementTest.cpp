//
// Created by kemal on 3/16/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"

TEST_CASE("Executes basic if statement", "[interpreter][controlFlow]") {
    std::wstring source = L"ako (istina) { ispisi 1; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 1);
}

TEST_CASE("Executes if-else statement", "[interpreter][controlFlow]") {
    std::wstring source = L"ako (neistina) { ispisi 1; } inace { ispisi 2; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}

TEST_CASE("Executes if-else if statement", "[interpreter][controlFlow]") {
    std::wstring source = L"ako (neistina) { ispisi 1; } inace ako (istina) { ispisi 2; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}