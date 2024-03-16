//
// Created by kemal on 3/16/2024.
//

#include "catch2/catch_test_macros.hpp"
#include "../TestHelpers.h"
#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "BlockStatement.h"
#include "VarDeclarationStatement.h"
#include "NumericLiteralExpression.h"

TEST_CASE("Parses empty block statement", "[parser][block]") {
    std::wstring source = L"{}";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto blockStatement = getNode<BlockStatement>(statements[0]);
    REQUIRE(blockStatement->statements.empty());
}

TEST_CASE("Parses block statement with one statement", "[parser][block]") {
    std::wstring source = L"{ var a = 5; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto statements = program->statements;

    auto blockStatement = getNode<BlockStatement>(statements[0]);
    REQUIRE(blockStatement->statements.size() == 1);
    auto varDeclaration = getNode<VarDeclarationStatement>(blockStatement->statements[0]);
    REQUIRE(varDeclaration->name->value == L"a");
    auto initializer = getNode<NumericLiteralExpression>(varDeclaration->initializer);
    REQUIRE(initializer->value == 5);
}

TEST_CASE("Throws on missing closing brace", "[parser][block]") {
    std::wstring source = L"{ var a = 5;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}
