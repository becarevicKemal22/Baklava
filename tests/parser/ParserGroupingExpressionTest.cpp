//
// Created by kemal on 3/12/2024.
//

#include "catch2/catch_test_macros.hpp"
#include "../TestHelpers.h"
#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "BinaryExpression.h"
#include "NumericLiteralExpression.h"
#include "GroupingExpression.h"

TEST_CASE("Groups basic expression", "[parser][grouping]"){
    std::wstring source = L"(1 + 2)";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto groupingExpression = getNode<GroupingExpression>(statements[0]);
    auto binaryExpression = getNode<BinaryExpression>(groupingExpression->expr);
    REQUIRE(binaryExpression->op->type == TokenType::Plus);
    auto left = getNode<NumericLiteralExpression>(binaryExpression->left);
    REQUIRE(left->value == 1);
    auto right = getNode<NumericLiteralExpression>(binaryExpression->right);
    REQUIRE(right->value == 2);
}

TEST_CASE("Groups nested grouping", "[parser][grouping]"){
    std::wstring source = L"(1 * (2 + 3))";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto groupingExpression = getNode<GroupingExpression>(statements[0]);
    auto binaryExpression = getNode<BinaryExpression>(groupingExpression->expr);
    REQUIRE(binaryExpression->op->type == TokenType::Star);
    auto left = getNode<NumericLiteralExpression>(binaryExpression->left);
    REQUIRE(left->value == 1);
    auto right = getNode<GroupingExpression>(binaryExpression->right);
    binaryExpression = getNode<BinaryExpression>(right->expr);
    REQUIRE(binaryExpression->op->type == TokenType::Plus);
    auto rightLeft = getNode<NumericLiteralExpression>(binaryExpression->left);
    REQUIRE(rightLeft->value == 2);
    auto rightRight = getNode<NumericLiteralExpression>(binaryExpression->right);
    REQUIRE(rightRight->value == 3);
}

TEST_CASE("Groups part of expression", "[parser][grouping]"){
    std::wstring source = L"1 + (2 * 3) + 4";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto binaryExpression = getNode<BinaryExpression>(statements[0]);
    REQUIRE(binaryExpression->op->type == TokenType::Plus);
    auto left = getNode<BinaryExpression>(binaryExpression->left);
    REQUIRE(left->op->type == TokenType::Plus);
    auto leftLeft = getNode<NumericLiteralExpression>(left->left);
    REQUIRE(leftLeft->value == 1);
    auto leftRight = getNode<GroupingExpression>(left->right);
    auto right = getNode<NumericLiteralExpression>(binaryExpression->right);
    REQUIRE(right->value == 4);
    binaryExpression = getNode<BinaryExpression>(leftRight->expr);
    REQUIRE(binaryExpression->op->type == TokenType::Star);
    auto rightLeft = getNode<NumericLiteralExpression>(binaryExpression->left);
    REQUIRE(rightLeft->value == 2);
    auto rightRight = getNode<NumericLiteralExpression>(binaryExpression->right);
    REQUIRE(rightRight->value == 3);
}

TEST_CASE("Throws on empty grouping expression", "[parser][grouping]") {
    std::wstring source = L"()";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Throws on invalid expression inside parenthesis", "[parser][grouping]") {
    std::wstring source = L"(1 + )";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Throws on incomplete grouping expression", "[parser][grouping]") {
    std::wstring source = L"(1";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Throws on incomplete grouping expression with operator", "[parser][grouping]") {
    std::wstring source = L"(1 +";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Throws on incomplete grouping expression with right operand", "[parser][grouping]") {
    std::wstring source = L"(1 + 2";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS(parser.parse());
}