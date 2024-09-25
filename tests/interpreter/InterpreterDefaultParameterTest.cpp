//
// Created by kemal on 9/25/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"
#include "Resolver.h"

TEST_CASE("Passes default arguments to function", "[interpreter][defaultParameters]") {
    std::wstring source = L"funkcija f(a=5) { ispiši a; } f();";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Passes second default parameter to function", "[interpreter][defaultParameters]") {
    std::wstring source = L"funkcija f(a=5, b=10) { ispiši b; } f();";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 10);
}

TEST_CASE("Correctly passes missing parameters when some optional params are filled out", "[interpreter][defaultParameters]") {
    std::wstring source = L"funkcija f(a, b=10, c=15) { ispiši a; ispiši b; ispisi c;} f(12, 20);";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 3);
    REQUIRE(interpreter.printHistory[0].as.number == 12);
    REQUIRE(interpreter.printHistory[1].as.number == 20);
    REQUIRE(interpreter.printHistory[2].as.number == 15);
}

TEST_CASE("Doesn't overwrite object default parameters", "[interpreter][defaultParameters]") {
    std::wstring source = L"funkcija f(a = [0]){ ispisi a[0]; a[0] = 10; } f(); f();";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 2);
    REQUIRE(interpreter.printHistory[0].as.number == 0);
    REQUIRE(interpreter.printHistory[1].as.number == 0);
}
