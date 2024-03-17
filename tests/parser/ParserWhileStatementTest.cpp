//
// Created by kemal on 3/16/2024.
//

#include "catch2/catch_test_macros.hpp"
#include "../TestHelpers.h"
#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "WhileStatement.h"
#include "BlockStatement.h"
#include "PrintStatement.h"
#include "BooleanLiteralExpression.h"
#include "NumericLiteralExpression.h"
#include "StringLiteralExpression.h"

TEST_CASE("Parses empty while statement", "[parser][controlFlow]") {
    std::wstring source = L"dok (istina) {}";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto whileStatement = getNode<WhileStatement>(program->statements[0]);
    auto condition = getNode<BooleanLiteralExpression>(whileStatement->condition);
    REQUIRE(condition->value == true);
    REQUIRE(whileStatement->body->type == AstNodeType::BlockStatement);
    auto block = getNode<BlockStatement>(whileStatement->body);
    REQUIRE(block->statements.size() == 0);
}

TEST_CASE("Parses while statement with statement", "[parser][controlFlow]") {
    std::wstring source = L"dok (istina) { ispisi 1; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto whileStatement = getNode<WhileStatement>(program->statements[0]);
    auto condition = getNode<BooleanLiteralExpression>(whileStatement->condition);
    REQUIRE(condition->value == true);
    REQUIRE(whileStatement->body->type == AstNodeType::BlockStatement);
    auto block = getNode<BlockStatement>(whileStatement->body);
    REQUIRE(block->statements.size() == 1);
    auto printStatement = getNode<PrintStatement>(block->statements[0]);
    auto literal = getNode<NumericLiteralExpression>(printStatement->expr);
    REQUIRE(literal->value == 1);
}

TEST_CASE("Parses while statement with no block", "[parser][controlFlow]") {
    std::wstring source = L"dok (istina) ispisi 1; ispisi \"Izvan petlje\";";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 2);
    auto whileStatement = getNode<WhileStatement>(program->statements[0]);
    auto condition = getNode<BooleanLiteralExpression>(whileStatement->condition);
    REQUIRE(condition->value == true);
    REQUIRE(whileStatement->body->type == AstNodeType::PrintStatement);
    auto printStatement = getNode<PrintStatement>(whileStatement->body);
    auto literal = getNode<NumericLiteralExpression>(printStatement->expr);
    REQUIRE(literal->value == 1);
    REQUIRE(program->statements[1]->type == AstNodeType::PrintStatement);
    printStatement = getNode<PrintStatement>(program->statements[1]);
    auto stringLiteral = getNode<StringLiteralExpression>(printStatement->expr);
    REQUIRE(stringLiteral->value == L"Izvan petlje");
}

TEST_CASE("Throws on missing condition", "[parser][controlFlow]") {
    std::wstring source = L"dok() {}";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on missing body", "[parser][controlFlow]") {
    std::wstring source = L"dok(istina)";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on missing condition and body", "[parser][controlFlow]") {
    std::wstring source = L"dok()";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on no opening parenthesis", "[parser][controlFlow]") {
    std::wstring source = L"dok istina) {}";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on no closing parenthesis", "[parser][controlFlow]") {
    std::wstring source = L"dok (istina {}";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}
