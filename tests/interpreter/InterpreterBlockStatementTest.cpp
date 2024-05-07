//
// Created by kemal on 3/16/2024.
//

#include "catch2/catch_test_macros.hpp"

#include "Interpreter.h"
#include "Program.h"
#include "Object.h"
#include "../TestHelpers.h"
#include "Resolver.h"

TEST_CASE("Interprets block statement", "[interpreter][block]") {
    std::wstring source = L"{ var x = 10; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.executedStatements.size() == 2);
    REQUIRE(interpreter.executedStatements[1]->type == AstNodeType::VarDeclarationStatement);
}

TEST_CASE("Interprets block statement with multiple statements", "[interpreter][block]") {
    std::wstring source = L"{ var x = 10; ispiši 1; }";
    Interpreter interpreter;
    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.executedStatements.size() == 3);
    REQUIRE(interpreter.executedStatements[1]->type == AstNodeType::VarDeclarationStatement);
    REQUIRE(interpreter.executedStatements[2]->type == AstNodeType::PrintStatement);
    REQUIRE(interpreter.printHistory.size() == 1);
    REQUIRE(interpreter.printHistory[0].as.number == 1);
}

TEST_CASE("Nesting and shadowing test", "[interpreter][block]") {
    std::wstring source = L"var a = \"global a\";\n"
                          "var b = \"global b\";\n"
                          "var c = \"global c\";\n"
                          "{\n"
                          " var a = \"outer a\";\n"
                          " var b = \"outer b\";\n"
                          " {\n"
                          " var a = \"inner a\";\n"
                          " ispiši a;\n"
                          " ispiši b;\n"
                          " ispiši c;\n"
                          " }\n"
                          " ispiši a;\n"
                          " ispiši b;\n"
                          " ispiši c;\n"
                          "}\n"
                          "ispiši a;\n"
                          "ispiši b;\n"
                          "ispiši c;";
    Interpreter interpreter;
    std::unique_ptr<Program> program = parseSource(source);
    Resolver resolver(&interpreter);
    resolver.resolve(program);
    interpreter.interpret(program.get());
    REQUIRE(interpreter.printHistory.size() == 9);
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[0]) == L"inner a");
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[1]) == L"outer b");
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[2]) == L"global c");
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[3]) == L"outer a");
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[4]) == L"outer b");
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[5]) == L"global c");
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[6]) == L"global a");
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[7]) == L"global b");
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.printHistory[8]) == L"global c");
}

