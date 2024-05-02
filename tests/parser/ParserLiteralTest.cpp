//
// Created by kemal on 2/5/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "NumericLiteralExpression.h"
#include "BooleanLiteralExpression.h"
#include "StringLiteralExpression.h"
#include "NullLiteralExpression.h"
#include "ArrayLiteralExpression.h"
#include "../TestHelpers.h"

TEST_CASE("Parses literals", "[parser][literal]") {
    std::wstring source = L"24.2; istina; neistina; \"string\"; nula;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 5);
    auto statements = program->statements;

    auto numericLiteral = getNode<NumericLiteralExpression>(statements[0]);
    REQUIRE(numericLiteral->value == 24.2);

    auto booleanLiteral = getNode<BooleanLiteralExpression>(statements[1]);
    REQUIRE(booleanLiteral->value == true);

    booleanLiteral = getNode<BooleanLiteralExpression>(statements[2]);
    REQUIRE(booleanLiteral->value == false);

    auto stringLiteral = getNode<StringLiteralExpression>(statements[3]);
    REQUIRE(stringLiteral->value == L"string");

    auto nullLiteral = getNode<NullLiteralExpression>(statements[4]);
}

TEST_CASE("Parses empty string", "[parser][literal]") {
    std::wstring source = L"\"\";";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto stringLiteral = getNode<StringLiteralExpression>(statements[0]);
    REQUIRE(stringLiteral->value == L"");
}

TEST_CASE("Parses array literal", "[parser][literal]") {
    std::wstring source = L"[1, 2, 3];";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto arrayLiteral = getNode<ArrayLiteralExpression>(statements[0]);
    REQUIRE(arrayLiteral->elements.size() == 3);
    auto elements = arrayLiteral->elements;

    auto first = getNode<NumericLiteralExpression>(elements[0]);
    REQUIRE(first->value == 1);

    auto second = getNode<NumericLiteralExpression>(elements[1]);
    REQUIRE(second->value == 2);

    auto third = getNode<NumericLiteralExpression>(elements[2]);
    REQUIRE(third->value == 3);
}

TEST_CASE("Parses empty array literal", "[parser][literal]") {
    std::wstring source = L"[];";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto arrayLiteral = getNode<ArrayLiteralExpression>(statements[0]);
    REQUIRE(arrayLiteral->elements.empty());
}