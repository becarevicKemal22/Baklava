//
// Created by kemal on 3/11/2024.
//

#include "catch2/catch_test_macros.hpp"
#include "../TestHelpers.h"
#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "LogicalExpression.h"
#include "NumericLiteralExpression.h"

TEST_CASE("Parses logical and", "[parser][binary][logical]") {
    std::wstring source = L"1 && 2";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto binaryExpression = getNode<LogicalExpression>(statements[0]);
    REQUIRE(binaryExpression->op->type == TokenType::DoubleAmpersand);
    auto left = getNode<NumericLiteralExpression>(binaryExpression->left);
    REQUIRE(left->value == 1);
    auto right = getNode<NumericLiteralExpression>(binaryExpression->right);
    REQUIRE(right->value == 2);
}

TEST_CASE("Parses logical or", "[parser][binary][logical]") {
    std::wstring source = L"1 || 2";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto binaryExpression = getNode<LogicalExpression>(statements[0]);
    REQUIRE(binaryExpression->op->type == TokenType::DoublePipe);
    auto left = getNode<NumericLiteralExpression>(binaryExpression->left);
    REQUIRE(left->value == 1);
    auto right = getNode<NumericLiteralExpression>(binaryExpression->right);
    REQUIRE(right->value == 2);
}

TEST_CASE("Follows logical operator precedence", "[parser][binary][logical]"){
    std::wstring source = L"1 || 2 && 3";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto binaryExpression = getNode<LogicalExpression>(statements[0]);
    REQUIRE(binaryExpression->op->type == TokenType::DoublePipe);
    auto left = getNode<NumericLiteralExpression>(binaryExpression->left);
    REQUIRE(left->value == 1);
    auto right = getNode<LogicalExpression>(binaryExpression->right);
    REQUIRE(right->op->type == TokenType::DoubleAmpersand);
    auto rightLeft = getNode<NumericLiteralExpression>(right->left);
    REQUIRE(rightLeft->value == 2);
    auto rightRight = getNode<NumericLiteralExpression>(right->right);
    REQUIRE(rightRight->value == 3);
}