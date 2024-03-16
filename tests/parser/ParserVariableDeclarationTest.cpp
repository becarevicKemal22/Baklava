//
// Created by kemal on 3/13/2024.
//


#include <catch2/catch_test_macros.hpp>

#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "NumericLiteralExpression.h"
#include "VarDeclarationStatement.h"
#include "../TestHelpers.h"
#include "UninitializedConst.h"

TEST_CASE("Parses basic variable declaration with no initializer", "[parser][variableDeclaration]") {
    std::wstring source = L"var a;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto varDeclaration = getNode<VarDeclarationStatement>(statements[0]);
    REQUIRE(varDeclaration->name->value == L"a");
    REQUIRE(varDeclaration->initializer == nullptr);
    REQUIRE(!varDeclaration->isConst);
}

TEST_CASE("Parses variable declaration with initializer", "[parser][variableDeclaration]") {
    std::wstring source = L"var a = 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto varDeclaration = getNode<VarDeclarationStatement>(statements[0]);
    REQUIRE(varDeclaration->name->value == L"a");
    auto initializer = getNode<NumericLiteralExpression>(varDeclaration->initializer);
    REQUIRE(initializer->value == 5);
    REQUIRE(!varDeclaration->isConst);
}

TEST_CASE("Throws error when no identifier is present", "[parser][variableDeclaration]") {
    std::wstring source = L"var;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws error when no semicolon is present", "[parser][variableDeclaration]") {
    std::wstring source = L"var a = 5";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws error when no equal sign is present", "[parser][variableDeclaration]") {
    std::wstring source = L"var a 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Parses const variable declaration", "[parser][variableDeclaration]") {
    std::wstring source = L"konst var a = 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto varDeclaration = getNode<VarDeclarationStatement>(statements[0]);
    REQUIRE(varDeclaration->name->value == L"a");
    auto initializer = getNode<NumericLiteralExpression>(varDeclaration->initializer);
    REQUIRE(initializer->value == 5);
    REQUIRE(varDeclaration->isConst);
}

TEST_CASE("Throws on const variable declaration with no initializer", "[parser][variableDeclaration]") {
    std::wstring source = L"konst var a;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), UninitializedConst);
}
