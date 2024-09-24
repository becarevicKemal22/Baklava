//
// Created by kemal on 9/25/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "../TestHelpers.h"
#include "VariableExpression.h"
#include "FunctionDeclarationStatement.h"
#include "NumericLiteralExpression.h"
#include "InvalidDefaultArgumentPosition.h"

TEST_CASE("Parses function with default argument", "[parser][defaultArguments]"){
    std::wstring source = L"funkcija f(x = 5) { ispiši x; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    std::unique_ptr<Program> program = parser.parse();
    REQUIRE(program->statements.size() == 1);
    auto functionDeclarationStatement = getNode<FunctionDeclarationStatement>(program->statements[0]);
    REQUIRE(functionDeclarationStatement->name->value == L"f");
    REQUIRE(functionDeclarationStatement->parameters.size() == 1);
    REQUIRE(functionDeclarationStatement->parameters[0]->value == L"x");
    REQUIRE(functionDeclarationStatement->defaultArguments.size() == 1);
    auto numericLiteralExpression = getNode<NumericLiteralExpression>(functionDeclarationStatement->defaultArguments[0]);
    REQUIRE(numericLiteralExpression->value == 5);
}

TEST_CASE("Parses function with multiple default arguments", "[parser][defaultArguments]"){
    std::wstring source = L"funkcija f(x = 5, y = 10) { ispiši x; ispiši y; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    std::unique_ptr<Program> program = parser.parse();
    REQUIRE(program->statements.size() == 1);
    auto functionDeclarationStatement = getNode<FunctionDeclarationStatement>(program->statements[0]);
    REQUIRE(functionDeclarationStatement->name->value == L"f");
    REQUIRE(functionDeclarationStatement->parameters.size() == 2);
    REQUIRE(functionDeclarationStatement->parameters[0]->value == L"x");
    REQUIRE(functionDeclarationStatement->parameters[1]->value == L"y");
    REQUIRE(functionDeclarationStatement->defaultArguments.size() == 2);
    auto numericLiteralExpression = getNode<NumericLiteralExpression>(functionDeclarationStatement->defaultArguments[0]);
    REQUIRE(numericLiteralExpression->value == 5);
    numericLiteralExpression = getNode<NumericLiteralExpression>(functionDeclarationStatement->defaultArguments[1]);
    REQUIRE(numericLiteralExpression->value == 10);
}

TEST_CASE("Parses function with mixed arguments", "[parser][defaultArguments]"){
    std::wstring source = L"funkcija f(x, y, z = 10) { ispiši x; ispiši y; ispiši z; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    std::unique_ptr<Program> program = parser.parse();
    REQUIRE(program->statements.size() == 1);
    auto functionDeclarationStatement = getNode<FunctionDeclarationStatement>(program->statements[0]);
    REQUIRE(functionDeclarationStatement->name->value == L"f");
    REQUIRE(functionDeclarationStatement->parameters.size() == 3);
    REQUIRE(functionDeclarationStatement->parameters[0]->value == L"x");
    REQUIRE(functionDeclarationStatement->parameters[1]->value == L"y");
    REQUIRE(functionDeclarationStatement->parameters[2]->value == L"z");
    REQUIRE(functionDeclarationStatement->defaultArguments.size() == 1);
    auto numericLiteralExpression = getNode<NumericLiteralExpression>(functionDeclarationStatement->defaultArguments[0]);
    REQUIRE(numericLiteralExpression->value == 10);
}

TEST_CASE("Throws on default argument without parameter", "[parser][defaultArguments]"){
    std::wstring source = L"funkcija f( = 5) { ispiši x; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on default argument before non-default argument", "[parser][defaultArguments]"){
    std::wstring source = L"funkcija f(x = 5, y) { ispiši x; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), InvalidDefaultArgumentPosition);
}

TEST_CASE("Throws on default argument is not primitive/literal/constant", "[parser][defaultArguments]"){
    std::wstring source = L"funkcija f(x = 5 + 5) { ispiši x; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on default argument is not primitive/literal/constant 2", "[parser][defaultArguments]"){
    std::wstring source = L"funkcija f(x = x) { ispiši x; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

// This is because if I were to allow parenthesized expressions, I would also have to check whether whatever is inside it is a primitive value
TEST_CASE("Throws when default argument is parenthesized expression", "[parser][defaultArguments]"){
    std::wstring source = L"funkcija f(x = (5)) { ispiši x; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}