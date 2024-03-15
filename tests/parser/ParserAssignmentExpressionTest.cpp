//
// Created by kemal on 3/14/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "../TestHelpers.h"
#include "VariableExpression.h"
#include "AssignmentExpression.h"
#include "NumericLiteralExpression.h"

TEST_CASE("Parses variable assignment", "[parser][assignmentExpression]"){
    std::wstring source = L"myVar = 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    std::unique_ptr<Program> program = parser.parse();
    REQUIRE(program->statements.size() == 1);

    auto assignmentExpression = getNode<AssignmentExpression>(program->statements[0]);
    REQUIRE(assignmentExpression->name->value == L"myVar");
    auto numericLiteralExpression = getNode<NumericLiteralExpression>(assignmentExpression->value);
    REQUIRE(numericLiteralExpression->value == 5);
}

TEST_CASE("Throws on no assignment value", "[parser][assignmentExpression]"){
    std::wstring source = L"myVar = ;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on no identifier", "[parser][assignmentExpression]"){
    std::wstring source = L"= 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ParserError);
}

TEST_CASE("Throws on no semicolon", "[parser][assignmentExpression]"){
    std::wstring source = L"myVar = 5";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on no equal sign", "[parser][assignmentExpression]"){
    std::wstring source = L"myVar 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on invalid lvalue", "[parser][assignmentExpression]"){
    std::wstring source = L"5 = 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), InvalidLValue);
}
