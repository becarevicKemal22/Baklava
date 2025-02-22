//
// Created by kemal on 6/7/2024.
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

TEST_CASE("Creates and accesses array", "[interpreter][indexing][array]") {
    std::wstring source = L"var a = [1, 2, 3]; ispiši a[2];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 3);
}

TEST_CASE("Creates and accesses array with expressions", "[interpreter][indexing][array]") {
    std::wstring source = L"var a = [1, 2, 3]; ispiši a[1 + 1];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 3);
}

TEST_CASE("Creates and accesses array with variable", "[interpreter][indexing][array]") {
    std::wstring source = L"var a = [1, 2, 3]; var j = 1; ispiši a[j];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}

TEST_CASE("Creates two-dimensional array and accesses it", "[interpreter][indexing][array]") {
    std::wstring source = L"var a = [[1, 2], [3, 4]]; ispiši a[1][0];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 3);
}

TEST_CASE("Shadows arrays correctly", "[interpreter][indexing][array]") {
    std::wstring source = L"var a = [1, 2, 3]; { var a = [4, 5, 6]; ispiši a[1]; } ispiši a[1];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 2);
    REQUIRE(interpreter.printHistory[0].as.number == 5);
    REQUIRE(interpreter.printHistory[1].as.number == 2);
}

TEST_CASE("Indexes array returned from function correctly", "[interpreter][indexing][array]") {
    // additional scope added just for a larger chance of failure
    std::wstring source = L"funkcija getArray() { vrati [1, 2, 3]; } { ispiši getArray()[1]; }";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 2);
}

TEST_CASE("Gives error on out-of-bounds array access", "[interpreter][indexing][array]") {
    std::wstring source = L"var a = [1, 2, 3]; ispiši a[3];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    REQUIRE_NOTHROW(interpreter.interpret(program.get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<IndexOutOfBounds>(&interpreter);
}

TEST_CASE("Gives error on negative index array access", "[interpreter][indexing][array]") {
    std::wstring source = L"var a = [1, 2, 3]; var b = -2; ispiši a[b];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    REQUIRE_NOTHROW(interpreter.interpret(program.get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<IndexOutOfBounds>(&interpreter);
}

TEST_CASE("Gives error on non-integer index array access", "[interpreter][indexing][array]") {
    std::wstring source = L"var a = [1, 2, 3]; var b = 2.5; ispiši a[b];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    REQUIRE_NOTHROW(interpreter.interpret(program.get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<NonIntegerIndex>(&interpreter);
}

TEST_CASE("Gives error on indexing of non-array type", "[interpreter][indexing][array]") {
    std::wstring source = L"var a = 5; ispiši a[0];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    REQUIRE_NOTHROW(interpreter.interpret(program.get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<IndexingNonArray>(&interpreter);
}

// this one is necessary just in case I break the check for obj and array obj.
// its commented in the evaluateIndexAssignmentExpression function. Basically, it has to use || operator instead of &&
// to prevent segfaults, but I have this just in case I ignore my comment.
TEST_CASE("Gives error on indexing of non-array type (object version)", "[interpreter][indexing][array]") {
    std::wstring source = L"var a = \"string\"; ispiši a[0];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    REQUIRE_NOTHROW(interpreter.interpret(program.get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<IndexingNonArray>(&interpreter);
}

TEST_CASE("Gives error on non-number index array access", "[interpreter][indexing][array]") {
    std::wstring source = L"var a = [1, 2, 3]; var b = \"string\"; ispiši a[b];";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source, &interpreter);
    REQUIRE_NOTHROW(interpreter.interpret(program.get()));
    REQUIRE(interpreter.hadError);
    checkHandledError<WrongTypeError>(&interpreter);
}