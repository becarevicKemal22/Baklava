//
// Created by kemal on 25-Feb-25.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "WrongTypeToStatement.h"
#include "../TestHelpers.h"

TEST_CASE("Executes basic increment statement", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = 1; povecaj a; ispisi a;";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}

TEST_CASE("Executes basic decrement statement", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = 0; smanji a; ispisi a;";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == -1);
}

TEST_CASE("Executes increment statement with 'za'", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = 5; povecaj a za 5.5; ispiši a;";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE_THAT(interpreter.printHistory[0].as.number, Catch::Matchers::WithinRel(10.5, 0.0001));
}

TEST_CASE("Executes decrement statement with 'za'", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = 5; smanji a za 5.5; ispiši a;";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE_THAT(interpreter.printHistory[0].as.number, Catch::Matchers::WithinRel(-0.5, 0.0001));
}

TEST_CASE("Executes increment statement with function call as 'za' value", "[interpreter][modifyStatement]") {
    std::wstring source = L"funkcija f(){vrati 5;}"
            "var a = 5; povecaj a za f(); ispiši a;";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 10);
}

TEST_CASE("Executes increment statement with array indexing", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = [1, -2, 3]; povecaj a[0]; ispiši a[0];";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}

TEST_CASE("Executes increment statement with array indexing as increment amount", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = [1, -2, 3]; var b = 0; za svako j od 0 do 3 povecaj b za a[j]; ispiši b;";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source, &interpreter).get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}

TEST_CASE("Throws indexing non-array error when incrementing non-array", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = 5; povecaj a[0];";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source, &interpreter).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<IndexingNonArray>(&interpreter);
}

TEST_CASE("Throws indexing out of bounds error when incrementing out of bounds", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = [1, -2, 3]; povecaj a[3];";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source, &interpreter).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<IndexOutOfBounds>(&interpreter);
}

TEST_CASE("Throws non-integer index error when incrementing with non-integer index", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = [1, -2, 3]; povecaj a[1.5];";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source, &interpreter).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<NonIntegerIndex>(&interpreter);
}

TEST_CASE("Throws on non-number index value", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = [1, -2, 3]; var b = \"0\"; povecaj a[b];";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source, &interpreter).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<WrongTypeError>(&interpreter);
}

// This error actually happens in the parser, as the interpreter assumes lvalue is ok
TEST_CASE("Throws invalid lvalue error when incrementing with non-lvalue", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = 5; povecaj 5;";
    Interpreter interpreter;
    // Throws as because its in the parser and doesn't get caught / gets rethrown in parseSource
    REQUIRE_THROWS_AS(interpreter.interpret(parseSource(source, &interpreter).get()), InvalidLValue);
}

TEST_CASE("Throws on non-number increment value", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = 5; var b = \"5\"; povecaj a za b;";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source, &interpreter).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<WrongTypeToStatement>(&interpreter);
}

TEST_CASE("Throws on non-number variable to be incremented", "[interpreter][modifyStatement]") {
    std::wstring source = L"var a = [1, 'str']; povecaj a[1];";
    Interpreter interpreter;
    REQUIRE_NOTHROW(interpreter.interpret(parseSource(source, &interpreter).get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<WrongTypeToStatement>(&interpreter);
}