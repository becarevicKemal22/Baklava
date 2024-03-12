//
// Created by kemal on 2/6/2024.
//

#include "catch2/catch_test_macros.hpp"
#include "../TestHelpers.h"
#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "BinaryExpression.h"
#include "NumericLiteralExpression.h"

TEST_CASE("Parses binary expressions", "[parser][binary]") {
    std::wstring source = L"1 + 2 - 3 * 4 / 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto binaryExpression = getNode<BinaryExpression>(statements[0]);
    REQUIRE(binaryExpression->op->type == TokenType::Minus);
    auto left = getNode<BinaryExpression>(binaryExpression->left);
    REQUIRE(left->op->type == TokenType::Plus);
    auto right = getNode<BinaryExpression>(binaryExpression->right);
    REQUIRE(right->op->type == TokenType::Slash);
    auto rightLeft = getNode<BinaryExpression>(right->left);
    REQUIRE(rightLeft->op->type == TokenType::Star);
    auto rightRight = getNode<NumericLiteralExpression>(right->right);
    REQUIRE(rightRight->value == 5);
    auto leftLeft = getNode<NumericLiteralExpression>(left->left);
    REQUIRE(leftLeft->value == 1);
    auto leftRight = getNode<NumericLiteralExpression>(left->right);
    REQUIRE(leftRight->value == 2);
}

TEST_CASE("Follows precedence", "[parser][binary]"){
    std::wstring source = L"1 + 2 * 3;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto binaryExpression = getNode<BinaryExpression>(statements[0]);
    REQUIRE(binaryExpression->op->type == TokenType::Plus);
    auto left = getNode<NumericLiteralExpression>(binaryExpression->left);
    REQUIRE(left->value == 1);
    auto right = getNode<BinaryExpression>(binaryExpression->right);
    REQUIRE(right->op->type == TokenType::Star);
    auto rightLeft = getNode<NumericLiteralExpression>(right->left);
    REQUIRE(rightLeft->value == 2);
    auto rightRight = getNode<NumericLiteralExpression>(right->right);
    REQUIRE(rightRight->value == 3);
}
