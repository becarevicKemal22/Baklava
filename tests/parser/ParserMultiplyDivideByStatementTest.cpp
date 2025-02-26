//
// Created by kemal on 26-Feb-25.
//

#include "catch2/catch_test_macros.hpp"
#include "../TestHelpers.h"
#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "ModifyStatement.h"
#include "IndexingExpression.h"
#include "NumericLiteralExpression.h"
#include "VariableExpression.h"
#include "CallExpression.h"

// Both this and ParserIncrementDecrementStatementTest are related to ModifyStatement, but are split into two functions as
// they are in the parser.

TEST_CASE("Parses multiply by", "[parser][modifyStatement]") {
    std::wstring source = L"pomnoži a sa 2;";
    Interpreter interpreter;
    auto program = parseSource(source, &interpreter);

    REQUIRE(program->statements.size() == 1);

    REQUIRE(program->statements[0]->type == AstNodeType::ModifyStatement);
    auto multiplyBy = getNode<ModifyStatement>(program->statements[0]);

    REQUIRE(multiplyBy->keyword->type == TokenType::Pomnozi);
    REQUIRE(multiplyBy->keyword->value == L"pomnoži");

    REQUIRE(multiplyBy->lvalue->type == AstNodeType::VariableExpression);
    auto lvalue = getNode<VariableExpression>(multiplyBy->lvalue);
    REQUIRE(lvalue->name->value == L"a");

    REQUIRE(multiplyBy->by->type == AstNodeType::NumericLiteralExpression);
    auto by = getNode<NumericLiteralExpression>(multiplyBy->by);
    REQUIRE(by->value == 2);

    REQUIRE(multiplyBy->modificationType == MULTIPLY);
}

TEST_CASE("Parses divide by", "[parser][modifyStatement]") {
    std::wstring source = L"podijeli a sa 2.2;";
    Interpreter interpreter;
    auto program = parseSource(source, &interpreter);

    REQUIRE(program->statements.size() == 1);

    REQUIRE(program->statements[0]->type == AstNodeType::ModifyStatement);
    auto divideBy = getNode<ModifyStatement>(program->statements[0]);

    REQUIRE(divideBy->keyword->type == TokenType::Podijeli);
    REQUIRE(divideBy->keyword->value == L"podijeli");

    REQUIRE(divideBy->lvalue->type == AstNodeType::VariableExpression);
    auto lvalue = getNode<VariableExpression>(divideBy->lvalue);
    REQUIRE(lvalue->name->value == L"a");

    REQUIRE(divideBy->by->type == AstNodeType::NumericLiteralExpression);
    auto by = getNode<NumericLiteralExpression>(divideBy->by);
    REQUIRE(by->value == 2.2);

    REQUIRE(divideBy->modificationType == DIVIDE);
}

TEST_CASE("Parses multiply by with function call as multiplicator", "[parser][modifyStatement]") {
    std::wstring source = L"pomnoži a sa funk();";
    Interpreter interpreter;
    auto program = parseSource(source, &interpreter);

    REQUIRE(program->statements.size() == 1);

    REQUIRE(program->statements[0]->type == AstNodeType::ModifyStatement);
    auto multiplyBy = getNode<ModifyStatement>(program->statements[0]);

    REQUIRE(multiplyBy->keyword->type == TokenType::Pomnozi);
    REQUIRE(multiplyBy->keyword->value == L"pomnoži");

    REQUIRE(multiplyBy->lvalue->type == AstNodeType::VariableExpression);
    auto lvalue = getNode<VariableExpression>(multiplyBy->lvalue);
    REQUIRE(lvalue->name->value == L"a");

    REQUIRE(multiplyBy->by->type == AstNodeType::CallExpression);
    auto by = getNode<CallExpression>(multiplyBy->by);
    REQUIRE(by->arguments.empty());

    REQUIRE(multiplyBy->modificationType == MULTIPLY);
}

TEST_CASE("Parses divide by with arrays", "[parser][modifyStatement]") {
    std::wstring source = L"podijeli a[5] sa b[1];";
    Interpreter interpreter;
    auto program = parseSource(source, &interpreter);

    REQUIRE(program->statements.size() == 1);

    REQUIRE(program->statements[0]->type == AstNodeType::ModifyStatement);
    auto divideBy = getNode<ModifyStatement>(program->statements[0]);

    REQUIRE(divideBy->keyword->type == TokenType::Podijeli);
    REQUIRE(divideBy->keyword->value == L"podijeli");

    REQUIRE(divideBy->lvalue->type == AstNodeType::IndexingExpression);
    auto lvalue = getNode<IndexingExpression>(divideBy->lvalue);
    REQUIRE(lvalue->left->type == AstNodeType::VariableExpression);
    auto left = getNode<VariableExpression>(lvalue->left);
    REQUIRE(left->name->value == L"a");
    REQUIRE(lvalue->index->type == AstNodeType::NumericLiteralExpression);
    auto index = getNode<NumericLiteralExpression>(lvalue->index);
    REQUIRE(index->value == 5);

    REQUIRE(divideBy->by->type == AstNodeType::IndexingExpression);
    auto by = getNode<IndexingExpression>(divideBy->by);
    REQUIRE(by->left->type == AstNodeType::VariableExpression);
    left = getNode<VariableExpression>(by->left);
    REQUIRE(left->name->value == L"b");
    REQUIRE(by->index->type == AstNodeType::NumericLiteralExpression);
    index = getNode<NumericLiteralExpression>(by->index);
    REQUIRE(index->value == 1);

    REQUIRE(divideBy->modificationType == DIVIDE);
}

TEST_CASE("Throws invalid lvalue error", "[parser][modifyStatement]") {
    std::wstring source = L"pomnoži 5 sa 2;";
    Interpreter interpreter;
    REQUIRE_THROWS_AS(parseSource(source, &interpreter), InvalidLValue);
}

TEST_CASE("Throws expected 'sa' error", "[parser][modifyStatement]") {
    std::wstring source = L"pomnoži a 2;";
    Interpreter interpreter;
    REQUIRE_THROWS_AS(parseSource(source, &interpreter), ExpectedXBeforeY);
}

TEST_CASE("Throws expected ';' error", "[parser][modifyStatement]") {
    std::wstring source = L"pomnoži a sa 2";
    Interpreter interpreter;
    REQUIRE_THROWS_AS(parseSource(source, &interpreter), ExpectedXBeforeY);
}
