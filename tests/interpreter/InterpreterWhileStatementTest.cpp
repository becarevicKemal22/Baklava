//
// Created by kemal on 3/16/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"
#include "Resolver.h"

TEST_CASE("Executes while loop", "[interpreter][controlFlow]") {
    std::wstring source = L"var x = 0; dok (x < 1) { ispiši x; x = x + 1; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 0);
}

TEST_CASE("Does not execute on initial false condition", "[interpreter][controlFlow]") {
    std::wstring source = L"dok netačno { ispiši 1; }";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source).get()));
    REQUIRE(interpreter.printHistory.empty());
}

TEST_CASE("Executes while loop without block", "[interpreter][controlFlow]") {
    std::wstring source = L"var x = 0; dok je x manje od 10 ponavljaj x = x + 1; ispiši x;"; // testing new type of syntax as well
    Interpreter interpreter;
    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 10);
}

TEST_CASE("Executes nested while loops", "[interpreter][controlFlow]") {
    std::wstring source = L"var x = 0; dok (x < 2) { var y = 0; dok (y < 2) { ispiši y; y = y + 1; } x = x + 1; }";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 4);
    REQUIRE(interpreter.printHistory[0].as.number == 0);
    REQUIRE(interpreter.printHistory[1].as.number == 1);
    REQUIRE(interpreter.printHistory[2].as.number == 0);
    REQUIRE(interpreter.printHistory[3].as.number == 1);
}
