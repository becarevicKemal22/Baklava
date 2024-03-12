//
// Created by kemal on 3/11/2024.
//

#include "catch2/catch_test_macros.hpp"
#include "../TestHelpers.h"
#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "BinaryExpression.h"
#include "NumericLiteralExpression.h"

TEST_CASE("Parses binary equality", "[parser][binary][comparison]") {
    std::wstring source = L"1 == 2 != 3;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto binaryExpression = getNode<BinaryExpression>(statements[0]);
    REQUIRE(binaryExpression->op->type == TokenType::BangEqual);
    auto left = getNode<BinaryExpression>(binaryExpression->left);
    REQUIRE(left->op->type == TokenType::DoubleEqual);
    auto right = getNode<NumericLiteralExpression>(binaryExpression->right);
    REQUIRE(right->value == 3);
    auto rightLeft = getNode<NumericLiteralExpression>(left->left);
    REQUIRE(rightLeft->value == 1);
    auto rightRight = getNode<NumericLiteralExpression>(left->right);
    REQUIRE(rightRight->value == 2);
}

TEST_CASE("Parses comparison operators", "[parser][binary][comparison]"){
    std::wstring source = L"1 < 2 > 3 <= 4 >= 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto binaryExpression = getNode<BinaryExpression>(statements[0]);
    REQUIRE(binaryExpression->op->type == TokenType::GreaterEqual);
    auto left = getNode<BinaryExpression>(binaryExpression->left);
    REQUIRE(left->op->type == TokenType::LessEqual);
    auto right = getNode<NumericLiteralExpression>(binaryExpression->right);
    REQUIRE(right->value == 5);
    auto left2 = getNode<BinaryExpression>(left->left);
    REQUIRE(left2->op->type == TokenType::Greater);
    auto right2 = getNode<NumericLiteralExpression>(left->right);
    REQUIRE(right2->value == 4);
    auto left3 = getNode<BinaryExpression>(left2->left);
    REQUIRE(left3->op->type == TokenType::Less);
    auto right3 = getNode<NumericLiteralExpression>(left2->right);
    REQUIRE(right3->value == 3);
    auto left4 = getNode<NumericLiteralExpression>(left3->left);
    REQUIRE(left4->value == 1);
    auto right4 = getNode<NumericLiteralExpression>(left3->right);
    REQUIRE(right4->value == 2);
}

TEST_CASE("Follows precedence for equality and comparison", "[parser][binary][comparison]"){
    std::wstring source = L"1 < 2 == 3;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto binaryExpression = getNode<BinaryExpression>(statements[0]);
    REQUIRE(binaryExpression->op->type == TokenType::DoubleEqual);
    auto left = getNode<BinaryExpression>(binaryExpression->left);
    REQUIRE(left->op->type == TokenType::Less);
    auto right = getNode<NumericLiteralExpression>(binaryExpression->right);
    REQUIRE(right->value == 3);
    auto leftLeft = getNode<NumericLiteralExpression>(left->left);
    REQUIRE(leftLeft->value == 1);
    auto leftRight = getNode<NumericLiteralExpression>(left->right);
    REQUIRE(leftRight->value == 2);
}