//
// Created by kemal on 25-Feb-25.
//

#include "catch2/catch_test_macros.hpp"
#include "../TestHelpers.h"
#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "IncrementStatement.h"
#include "IndexingExpression.h"
#include "NumericLiteralExpression.h"
#include "VariableExpression.h"

TEST_CASE("Parses increment statement", "[parser][increment]") {
    std::wstring source = L"povećaj a;";
    Interpreter interpreter;
    auto program = parseSource(source, &interpreter);

    REQUIRE(program->statements.size() == 1);

    REQUIRE(program->statements[0]->type == AstNodeType::IncrementStatement);
    auto incrementStatement = getNode<IncrementStatement>(program->statements[0]);

    REQUIRE(incrementStatement->keyword->type == TokenType::Povecaj);
    REQUIRE(incrementStatement->keyword->value == L"povećaj");

    REQUIRE(incrementStatement->lvalue->type == AstNodeType::VariableExpression);
    auto lvalue = getNode<VariableExpression>(incrementStatement->lvalue);
    REQUIRE(lvalue->name->value == L"a");

    REQUIRE(incrementStatement->by->type == AstNodeType::NumericLiteralExpression);
    auto by = getNode<NumericLiteralExpression>(incrementStatement->by);
    REQUIRE(by->value == 1);

    REQUIRE(incrementStatement->isDecrement == false);
}

TEST_CASE("Parses decrement statement", "[parser][increment]") {
    std::wstring source = L"smanji a;";
    Interpreter interpreter;
    auto program = parseSource(source, &interpreter);

    REQUIRE(program->statements.size() == 1);

    REQUIRE(program->statements[0]->type == AstNodeType::IncrementStatement);
    auto incrementStatement = getNode<IncrementStatement>(program->statements[0]);

    REQUIRE(incrementStatement->keyword->type == TokenType::Smanji);
    REQUIRE(incrementStatement->keyword->value == L"smanji");

    REQUIRE(incrementStatement->lvalue->type == AstNodeType::VariableExpression);
    auto lvalue = getNode<VariableExpression>(incrementStatement->lvalue);
    REQUIRE(lvalue->name->value == L"a");

    REQUIRE(incrementStatement->by->type == AstNodeType::NumericLiteralExpression);
    auto by = getNode<NumericLiteralExpression>(incrementStatement->by);
    REQUIRE(by->value == 1);

    REQUIRE(incrementStatement->isDecrement == true);
}

TEST_CASE("Parses increment statement with 'by'", "[parser][increment]") {
    std::wstring source = L"povecaj a za 2;";
    Interpreter interpreter;
    auto program = parseSource(source, &interpreter);

    REQUIRE(program->statements.size() == 1);

    REQUIRE(program->statements[0]->type == AstNodeType::IncrementStatement);
    auto incrementStatement = getNode<IncrementStatement>(program->statements[0]);

    REQUIRE(incrementStatement->lvalue->type == AstNodeType::VariableExpression);
    auto lvalue = getNode<VariableExpression>(incrementStatement->lvalue);
    REQUIRE(lvalue->name->value == L"a");

    REQUIRE(incrementStatement->by->type == AstNodeType::NumericLiteralExpression);
    auto by = getNode<NumericLiteralExpression>(incrementStatement->by);
    REQUIRE(by->value == 2);

    REQUIRE(incrementStatement->isDecrement == false);
}

TEST_CASE("Parses decrement statement with 'by'", "[parser][increment]") {
    std::wstring source = L"smanji a za 2;";
    Interpreter interpreter;
    auto program = parseSource(source, &interpreter);

    REQUIRE(program->statements.size() == 1);

    REQUIRE(program->statements[0]->type == AstNodeType::IncrementStatement);
    auto incrementStatement = getNode<IncrementStatement>(program->statements[0]);

    REQUIRE(incrementStatement->lvalue->type == AstNodeType::VariableExpression);
    auto lvalue = getNode<VariableExpression>(incrementStatement->lvalue);
    REQUIRE(lvalue->name->value == L"a");

    REQUIRE(incrementStatement->by->type == AstNodeType::NumericLiteralExpression);
    auto by = getNode<NumericLiteralExpression>(incrementStatement->by);
    REQUIRE(by->value == 2);

    REQUIRE(incrementStatement->isDecrement == true);
}

TEST_CASE("Parses increment statement with array indexing", "[parser][increment]") {
    std::wstring source = L"povecaj a[5] za b[1];";
    Interpreter interpreter;
    auto program = parseSource(source, &interpreter);

    REQUIRE(program->statements.size() == 1);

    REQUIRE(program->statements[0]->type == AstNodeType::IncrementStatement);
    auto incrementStatement = getNode<IncrementStatement>(program->statements[0]);

    REQUIRE(incrementStatement->lvalue->type == AstNodeType::IndexingExpression);
    auto lvalue = getNode<IndexingExpression>(incrementStatement->lvalue);
    REQUIRE(lvalue->left->type == AstNodeType::VariableExpression);
    auto left = getNode<VariableExpression>(lvalue->left);
    REQUIRE(left->name->value == L"a");
    REQUIRE(lvalue->index->type == AstNodeType::NumericLiteralExpression);
    auto index = getNode<NumericLiteralExpression>(lvalue->index);
    REQUIRE(index->value == 5);

    REQUIRE(incrementStatement->by->type == AstNodeType::IndexingExpression);
    auto by = getNode<IndexingExpression>(incrementStatement->by);
    REQUIRE(by->left->type == AstNodeType::VariableExpression);
    left = getNode<VariableExpression>(by->left);
    REQUIRE(left->name->value == L"b");
    REQUIRE(by->index->type == AstNodeType::NumericLiteralExpression);
    index = getNode<NumericLiteralExpression>(by->index);
    REQUIRE(index->value == 1);

    REQUIRE(incrementStatement->isDecrement == false);
}

TEST_CASE("Throws error if lvalue is not a variable", "[parser][increment]") {
    std::wstring source = L"povecaj a();";
    Interpreter interpreter;
    REQUIRE_THROWS_AS(parseSource(source, &interpreter), InvalidLValue);
}

TEST_CASE("Throws if no expression after 'za'", "[parser][increment]") {
    std::wstring source = L"povecaj a za;";
    Interpreter interpreter;
    REQUIRE_THROWS_AS(parseSource(source, &interpreter), ExpectedXBeforeY);
}

TEST_CASE("Throws on no semicolon after increment", "[parser][increment]") {
    std::wstring source = L"povecaj a";
    Interpreter interpreter;
    REQUIRE_THROWS_AS(parseSource(source, &interpreter), ExpectedXBeforeY);
}

TEST_CASE("Throws on no semicolon after 'za'", "[parser][increment]") {
    std::wstring source = L"povecaj a za 2";
    Interpreter interpreter;
    REQUIRE_THROWS_AS(parseSource(source, &interpreter), ExpectedXBeforeY);
}