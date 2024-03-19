//
// Created by kemal on 3/19/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"

TEST_CASE("Declares and calls basic function", "[interpreter][function]") {
    std::wstring source = L"funkcija f() { ispisi 5; } f();";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Declares and calls function with parameters", "[interpreter][function]") {
    std::wstring source = L"funkcija f(x) { ispisi x; } f(5);";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Declares and calls function with multiple parameters", "[interpreter][function]") {
    std::wstring source = L"funkcija f(x, y) { ispisi x; ispisi y; } f(5, 10);";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 2);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
    REQUIRE(interpreter.printHistory[1].as.number == 10);
}

TEST_CASE("Declares and calls function with return statement", "[interpreter][function]") {
    std::wstring source = L"funkcija f() { vrati 5; } ispisi f();";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Declares and calls function with return statement and parameters", "[interpreter][function]") {
    std::wstring source = L"funkcija f(x, y) { vrati x + y; } ispisi f(5, 10);";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 15);
}

TEST_CASE("Declares and calls function with return statement and nested function call", "[interpreter][function]") {
    std::wstring source = L"funkcija f(x, y) { vrati x + y; } ispisi f(5, f(5, 5));";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 15);
}

TEST_CASE("Declares and calls function inside a block", "[interpreter][function]") {
    std::wstring source = L"{ funkcija f() { ispisi 5; } f(); }";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Declares and calls function inside a block with shadowing", "[interpreter][function]") {
    std::wstring source = L"var x = 10; { funkcija f() { ispisi x; } f(); }";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 10);
}

TEST_CASE("Declares function outside block and calls inside block", "[interpreter][function]") {
    std::wstring source = L"funkcija f() { ispisi 5; } { f(); }";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Has error on string call", "[interpreter][function]") {
    std::wstring source = L"var f = \"string\"; f();";
    Interpreter interpreter;

    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<InvalidCall>(&interpreter);
}

TEST_CASE("Throws on function call with too many arguments", "[interpreter][function]") {
    std::wstring source = L"funkcija f() { } f(5);";
    Interpreter interpreter;

    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<InvalidArgumentCount>(&interpreter);
}

TEST_CASE("Throws on function call with too few arguments", "[interpreter][function]") {
    std::wstring source = L"funkcija f(x) { } f();";
    Interpreter interpreter;

    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<InvalidArgumentCount>(&interpreter);
}

TEST_CASE("Throws on number call", "[interpreter][function]") {
    std::wstring source = L"var f = 5; f();";
    Interpreter interpreter;

    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<InvalidCall>(&interpreter);
}

TEST_CASE("Throws on null call", "[interpreter][function]") {
    std::wstring source = L"var f = nula; f();";
    Interpreter interpreter;

    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<InvalidCall>(&interpreter);
}

TEST_CASE("Throws on boolean call", "[interpreter][function]") {
    std::wstring source = L"var f = istina; f();";
    Interpreter interpreter;

    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<InvalidCall>(&interpreter);
}
