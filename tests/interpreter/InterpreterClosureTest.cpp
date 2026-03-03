//
// Created by kemal on 03-Mar-26.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"
#include "Resolver.h"


TEST_CASE("Basic closure test", "[interpreter][closure]") {
    std::wstring source = L"funkcija f(){"
            "   var lok = 'Lokalna';"
            "   funkcija g(){ ispisi lok; }"
            "   vrati g;"
            "}"
            "var fja = f();"
            "fja();";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(IS_STRING_OBJ(interpreter.printHistory[0]));
    REQUIRE(AS_STRING_OBJ(interpreter.printHistory[0])->value == L"Lokalna");
}

TEST_CASE("Closure counter test", "[interpreter][closure]") {
    std::wstring source = L"funkcija makeCounter(){\n"
            "    var j = 0;\n"
            "    funkcija count(){\n"
            "        j = j + 1;\n"
            "        vrati j;\n"
            "    }\n"
            "    vrati count;\n"
            "}\n"
            "\n"
            "var counter = makeCounter();\n"
            "\n"
            "ispiši counter(); // 1\n"
            "ispiši counter(); // 2";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 2);
    REQUIRE(interpreter.printHistory[0].as.number == 1);
    REQUIRE(interpreter.printHistory[1].as.number == 2);
}

TEST_CASE("Closure counter shared state test", "[interpreter][closure]") {
    std::wstring source = L"funkcija makeCounter(){\n"
            "    var j = 0;\n"
            "    funkcija count(){\n"
            "        j = j + 1;\n"
            "        vrati j;\n"
            "    }\n"
            "    vrati count;\n"
            "}\n"
            "\n"
            "var counter1 = makeCounter();\n"
            "var counter2 = makeCounter();"
            "\n"
            "ispiši counter1(); // 1\n"
            "ispiši counter2(); // 2\n"
            "ispiši counter1(); // 3";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 3);
    REQUIRE(interpreter.printHistory[0].as.number == 1);
    REQUIRE(interpreter.printHistory[1].as.number == 2);
    REQUIRE(interpreter.printHistory[2].as.number == 3);
}

TEST_CASE("Closure adder test", "[interpreter][closure]") {
    std::wstring source = L"funkcija napraviSabirac(n){"
            "funkcija f(a){ vrati n + a; }"
            "vrati f;"
            "}"
            "var saberi5 = napraviSabirac(5);"
            "var saberi13 = napraviSabirac(13);"
            "ispisi saberi5(7);"
            "ispisi saberi13(7);";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 2);
    REQUIRE(interpreter.printHistory[0].as.number == 12);
    REQUIRE(interpreter.printHistory[1].as.number == 20);
}

TEST_CASE("Nested closures test", "[interpreter][closure]") {
    std::wstring source = L"funkcija suma1(a){"
            "funkcija suma2(b){"
            "funkcija suma3(c){"
            "funkcija suma4(d){"
            "  vrati a + b + c + d;"
            "}}}}"
            "ispisi suma1(10)(20)(30)(40);";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 100);
}

TEST_CASE("Closure keeps reference to enclosing env test", "[interpreter][closure]") {
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
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 2);
    REQUIRE(IS_STRING_OBJ(interpreter.printHistory[0]));
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[0]) == L"global");
    REQUIRE(IS_STRING_OBJ(interpreter.printHistory[1]));
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[1]) == L"local");
}

TEST_CASE("Closure keeps reference to enclosing env test 2", "[interpreter][closure]") {
    std::wstring source = L"funkcija f(){"
            "   var lok = 'Prva';"
            "   funkcija g(){"
            "      ispisi lok;"
            "   }"
            "   lok = 'Druga';"
            "   vrati g;"
            "}"
            "var fja = f();"
            "fja();";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(IS_STRING_OBJ(interpreter.printHistory[0]));
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[0]) == L"Druga");
}
