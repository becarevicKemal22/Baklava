//
// Created by kemal on 2/5/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Parser.h"
#include "UnaryExpression.h"
#include "Lexer.h"
#include "Program.h"
#include "BooleanLiteralExpression.h"
#include "NumericLiteralExpression.h"
#include "StringLiteralExpression.h"
#include "../TestHelpers.h"

TEST_CASE("Parses unary expressions", "[parser][unary]") {
    std::wstring source = L"-24 !istina";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 2);
    auto statements = program->statements;

    auto unaryExpression = getNode<UnaryExpression>(statements[0]);
    REQUIRE(unaryExpression->op->type == TokenType::Minus);
    auto numericLiteral = getNode<NumericLiteralExpression>(unaryExpression->expr);
    REQUIRE(numericLiteral->value == 24);

    unaryExpression = getNode<UnaryExpression>(statements[1]);
    REQUIRE(unaryExpression->op->type == TokenType::Bang);
    auto booleanLiteral = getNode<BooleanLiteralExpression>(unaryExpression->expr);
    REQUIRE(booleanLiteral->value == true);
}

TEST_CASE("Allows chaining of unary operators", "[parser][unary]"){
    std::wstring source = L"--24";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto unaryExpression = getNode<UnaryExpression>(statements[0]);
    REQUIRE(unaryExpression->op->type == TokenType::Minus);

    unaryExpression = getNode<UnaryExpression>(unaryExpression->expr);
    REQUIRE(unaryExpression->op->type == TokenType::Minus);
    auto numericLiteral = getNode<NumericLiteralExpression>(unaryExpression->expr);
    REQUIRE(numericLiteral->value == 24);
}

TEST_CASE("Parses string negation", "[parser][unary]"){
    std::wstring source = L"-\"string\"";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto unaryExpression = getNode<UnaryExpression>(statements[0]);
    REQUIRE(unaryExpression->op->type == TokenType::Minus);
    auto stringLiteral = getNode<StringLiteralExpression>(unaryExpression->expr);
    REQUIRE(stringLiteral->value == L"string");
}