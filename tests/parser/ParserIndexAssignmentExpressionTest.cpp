//
// Created by kemal on 6/7/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "../TestHelpers.h"
#include "VariableExpression.h"
#include "IndexAssignmentExpression.h"
#include "IndexingExpression.h"
#include "NumericLiteralExpression.h"

TEST_CASE("Parses basic index variable assignment", "[parser][indexAssignmentExpression]"){
    std::wstring source = L"myVar[1] = 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    std::unique_ptr<Program> program = parser.parse();
    REQUIRE(program->statements.size() == 1);

    auto indexAssignmentExpression = getNode<IndexAssignmentExpression>(program->statements[0]);
    auto variableExpression = getNode<VariableExpression>(indexAssignmentExpression->left);
    REQUIRE(variableExpression->name->value == L"myVar");
    auto index = indexAssignmentExpression->index;
    auto numericLiteralExpression = getNode<NumericLiteralExpression>(index);
    REQUIRE(numericLiteralExpression->value == 1);
    auto value = indexAssignmentExpression->value;
    auto numericLiteralExpression2 = getNode<NumericLiteralExpression>(value);
    REQUIRE(numericLiteralExpression2->value == 5);
}

TEST_CASE("Parses double index variable assignment", "[parser][indexAssignmentExpression]"){
    std::wstring source = L"myVar[1][2] = 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    std::unique_ptr<Program> program = parser.parse();
    REQUIRE(program->statements.size() == 1);

    auto outerIndexAssignmentExpression = getNode<IndexAssignmentExpression>(program->statements[0]);
    auto innerIndexingExpression = getNode<IndexingExpression>(outerIndexAssignmentExpression->left);
    auto variableExpression = getNode<VariableExpression>(innerIndexingExpression->left);
    REQUIRE(variableExpression->name->value == L"myVar");
    auto index = innerIndexingExpression->index;
    auto numericLiteralExpression = getNode<NumericLiteralExpression>(index);
    REQUIRE(numericLiteralExpression->value == 1);
    auto valueToAssign = outerIndexAssignmentExpression->value;
    auto numericLiteralExpression2 = getNode<NumericLiteralExpression>(valueToAssign);
    REQUIRE(numericLiteralExpression2->value == 5);
    auto outerIndex = outerIndexAssignmentExpression->index;
    auto numericLiteralExpression3 = getNode<NumericLiteralExpression>(outerIndex);
    REQUIRE(numericLiteralExpression3->value == 2);
}

TEST_CASE("Throws on no assignment value (index assignment)", "[parser][indexAssignmentExpression]"){
    std::wstring source = L"myVar[1] = ;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on no identifier (index assignment)", "[parser][indexAssignmentExpression]"){
    std::wstring source = L"[1] = 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ParserError);
}

TEST_CASE("Throws on no semicolon (index assignment)", "[parser][indexAssignmentExpression]"){
    std::wstring source = L"myVar[1] = 5";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}
