//
// Created by kemal on 10/1/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"
#include "Resolver.h"

TEST_CASE("Executes for loop", "[interpreter][controlFlow]") {
    std::wstring source = L"za svako x od 0 do 1 { ispiši x; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 0);
}

TEST_CASE("Executes for loop without block", "[interpreter][controlFlow]") {
    std::wstring source = L"za svako x od 0 do 1 ispiši x;";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 0);
}

TEST_CASE("Executes nested for loops (matrix printing example)", "[interpreter][controlFlow]") {
    std::wstring source = L"var matrica = [[1, 2], [3, 4]]; "
                          "za svako x od 0 do 2 { za svako y od 0 do 2 { ispisi matrica[x][y]; } }";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 4);
    REQUIRE(interpreter.printHistory[0].as.number == 1);
    REQUIRE(interpreter.printHistory[1].as.number == 2);
    REQUIRE(interpreter.printHistory[2].as.number == 3);
    REQUIRE(interpreter.printHistory[3].as.number == 4);
}

TEST_CASE("Executes for loop with step", "[interpreter][controlFlow]") {
    std::wstring source = L"za svako x od 0 do 6 korakom 2 { ispiši x; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 3);
    REQUIRE(interpreter.printHistory[0].as.number == 0);
    REQUIRE(interpreter.printHistory[1].as.number == 2);
    REQUIRE(interpreter.printHistory[2].as.number == 4);
}

// This is the test I was talking about in the parser test, it checks whether the interpreter switches the condition
// of the for loop to check if the variable is greater than the end.
TEST_CASE("Executes for loop with negative step", "[interpreter][controlFlow]") {
    std::wstring source = L"za svako x od 6 do 0 korakom -2 { ispiši x; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 3);
    REQUIRE(interpreter.printHistory[0].as.number == 6);
    REQUIRE(interpreter.printHistory[1].as.number == 4);
    REQUIRE(interpreter.printHistory[2].as.number == 2);
}

TEST_CASE("Executes for loop with negative step and end", "[interpreter][controlFlow]") {
    std::wstring source = L"za svako x od 6 do -3 korakom -2 { ispiši x; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 5);
    REQUIRE(interpreter.printHistory[0].as.number == 6);
    REQUIRE(interpreter.printHistory[1].as.number == 4);
    REQUIRE(interpreter.printHistory[2].as.number == 2);
    REQUIRE(interpreter.printHistory[3].as.number == 0);
    REQUIRE(interpreter.printHistory[4].as.number == -2);
}

TEST_CASE("Executes for loop with negative start and end", "[interpreter][controlFlow]") {
    std::wstring source = L"za svako x od -6 do -3 korakom 2 { ispiši x; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 2);
    REQUIRE(interpreter.printHistory[0].as.number == -6);
    REQUIRE(interpreter.printHistory[1].as.number == -4);
}

TEST_CASE("Is able to print array in reverse", "[interpreter][controlFlow]") {
    std::wstring source = L"var niz = [1, 2, 3, 4]; "
                          "za svako x od 3 do -1 korakom -1 { ispiši niz[x]; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 4);
    REQUIRE(interpreter.printHistory[0].as.number == 4);
    REQUIRE(interpreter.printHistory[1].as.number == 3);
    REQUIRE(interpreter.printHistory[2].as.number == 2);
    REQUIRE(interpreter.printHistory[3].as.number == 1);
}

TEST_CASE("Handles floating point increments and limits", "[interpreter][controlFlow]") {
    std::wstring source = L"za svako x od 0.15 do 0.9 korakom 0.1 { ispiši x; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 8);
    REQUIRE(interpreter.printHistory[0].as.number == 0.15);
    REQUIRE(interpreter.printHistory[1].as.number == 0.25);
    REQUIRE(interpreter.printHistory[2].as.number == 0.35);
    REQUIRE(interpreter.printHistory[3].as.number == 0.45);
    REQUIRE(interpreter.printHistory[4].as.number == 0.55);
    REQUIRE(interpreter.printHistory[5].as.number == 0.65);
    REQUIRE(interpreter.printHistory[6].as.number == 0.75);
    REQUIRE(interpreter.printHistory[7].as.number == 0.85);
}