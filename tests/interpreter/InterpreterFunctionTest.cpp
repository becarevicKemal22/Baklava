//
// Created by kemal on 3/19/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"
#include "Resolver.h"

TEST_CASE("Declares and calls basic function", "[interpreter][function]") {
    std::wstring source = L"funkcija f() { ispiši 5; } f();";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Declares and calls function with parameters", "[interpreter][function]") {
    std::wstring source = L"funkcija f(x) { ispiši x; } f(5);";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Declares and calls function with multiple parameters", "[interpreter][function]") {
    std::wstring source = L"funkcija f(x, y) { ispiši x; ispiši y; } f(5, 10);";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 2);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
    REQUIRE(interpreter.printHistory[1].as.number == 10);
}

TEST_CASE("Declares and calls function with return statement", "[interpreter][function]") {
    std::wstring source = L"funkcija f() { vrati 5; } ispiši f();";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Declares and calls function with return statement and parameters", "[interpreter][function]") {
    std::wstring source = L"funkcija f(x, y) { vrati x + y; } ispiši f(5, 10);";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 15);
}

TEST_CASE("Declares and calls function with return statement and nested function call", "[interpreter][function]") {
    std::wstring source = L"funkcija f(x, y) { vrati x + y; } ispiši f(5, f(5, 5));";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 15);
}

TEST_CASE("Declares and calls function inside a block", "[interpreter][function]") {
    std::wstring source = L"{ funkcija f() { ispiši 5; } f(); }";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Declares and calls function inside a block with shadowing", "[interpreter][function]") {
    std::wstring source = L"var x = 10; { funkcija f() { ispiši x; } f(); }";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 10);
}

TEST_CASE("Declares function outside block and calls inside block", "[interpreter][function]") {
    std::wstring source = L"funkcija f() { ispiši 5; } { f(); }";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Closure test", "[interpreter][function]") {
    std::wstring source = L"funkcija makeCounter(){\n"
                          "    var i = 0;\n"
                          "    funkcija count(){\n"
                          "        i = i + 1;\n"
                          "        vrati i;\n"
                          "    }\n"
                          "    vrati count;\n"
                          "}\n"
                          "\n"
                          "var counter = makeCounter();\n"
                          "\n"
                          "ispiši counter(); // 1\n"
                          "ispiši counter(); // 2";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 2);
    REQUIRE(interpreter.printHistory[0].as.number == 1);
    REQUIRE(interpreter.printHistory[1].as.number == 2);
}

TEST_CASE("Closure edge case test", "[interpreter][function]") {
    std::wstring source = L"var a = \"global\";\n"
                          "{\n"
                          "    funkcija fn(){\n"
                          "        ispiši a;\n"
                          "    }\n"
                          "    fn();\n"
                          "    var a = \"local\";\n"
                          "    fn();\n"
                          "}";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 2);
    REQUIRE(IS_STRING_OBJ(interpreter.printHistory[0]));
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[0]) == L"global");
    REQUIRE(IS_STRING_OBJ(interpreter.printHistory[1]));
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[1]) == L"global");
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
    std::wstring source = L"var f = null; f();";
    Interpreter interpreter;

    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<InvalidCall>(&interpreter);
}

TEST_CASE("Throws on boolean call", "[interpreter][function]") {
    std::wstring source = L"var f = tačno; f();";
    Interpreter interpreter;

    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<InvalidCall>(&interpreter);
}
