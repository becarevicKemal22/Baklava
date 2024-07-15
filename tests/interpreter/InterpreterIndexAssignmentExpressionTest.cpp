//
// Created by kemal on 6/8/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "../TestHelpers.h"
#include "Resolver.h"
#include "IndexOutOfBounds.h"
#include "NonIntegerIndex.h"
#include "IndexingNonArray.h"

TEST_CASE("Modifies array index", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = [1, 2, 3]; a[1] = 5; ispiši a[1];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Modifies array index with expressions", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = [1, 2, 3]; a[1 + 1] = 5; ispiši a[2];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Modifies matrix index", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = [[1, 2], [3, 4]]; a[1][0] = 5; ispiši a[1][0];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Modifies matrix index with expressions", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = [[1, 2], [3, 4]]; a[1][1 - 1] = 5; ispiši a[1][0];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Modifies array index with variable", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = [1, 2, 3]; var i = 1; a[i] = 5; ispiši a[1];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Modifies matrix index with variable", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = [[1, 2], [3, 4]]; var i = 1; a[i][0] = 5; ispiši a[1][0];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Correctly assigns when shadowing", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = [1, 2, 3]; { var a = [4, 5, 6]; a[1] = 10; ispiši a[1]; } ispiši a[1];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 2);
    REQUIRE(interpreter.printHistory[0].as.number == 10);
    REQUIRE(interpreter.printHistory[1].as.number == 2);
}

TEST_CASE("Assigns to array reference in function (pass by reference test)", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = [1, 2, 3]; funkcija f(arr) { arr[1] = 5; } f(a); ispiši a[1];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
}

TEST_CASE("Gives error on out-of-bounds array index assignment", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = [1, 2, 3]; a[3] = 5;";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    REQUIRE_NOTHROW(interpreter.interpret(program.get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<IndexOutOfBounds>(&interpreter);
}

TEST_CASE("Gives error on non-integer array index assignment", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = [1, 2, 3]; a[\"string\"] = 5;";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    REQUIRE_NOTHROW(interpreter.interpret(program.get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<WrongTypeError>(&interpreter);
}

TEST_CASE("Gives error on non-array index assignment", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = 5; a[0] = 5;";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    REQUIRE_NOTHROW(interpreter.interpret(program.get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<IndexingNonArray>(&interpreter);
}

TEST_CASE("Gives error on out-of-bounds matrix index assignment", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = [[1, 2], [3, 4]]; a[1][2] = 5;";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    REQUIRE_NOTHROW(interpreter.interpret(program.get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<IndexOutOfBounds>(&interpreter);
}

TEST_CASE("Gives error on non-integer matrix index assignment", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = [[1, 2], [3, 4]]; a[1][\"string\"] = 5;";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    REQUIRE_NOTHROW(interpreter.interpret(program.get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<WrongTypeError>(&interpreter);
}

TEST_CASE("Gives error on non-array matrix index assignment", "[interpreter][indexAssignment][array]") {
    std::wstring source = L"var a = 5; a[0][0] = 5;";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    REQUIRE_NOTHROW(interpreter.interpret(program.get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<IndexingNonArray>(&interpreter);
}
