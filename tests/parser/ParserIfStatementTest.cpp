//
// Created by kemal on 3/16/2024.
//

#include "catch2/catch_test_macros.hpp"
#include "../TestHelpers.h"
#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "IfStatement.h"
#include "BlockStatement.h"
#include "PrintStatement.h"
#include "BooleanLiteralExpression.h"
#include "NumericLiteralExpression.h"

TEST_CASE("Parses basic if statement", "[parser][controlFlow]") {
    std::wstring source = L"ako (istina) { ispisi 1; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto ifStatement = getNode<IfStatement>(program->statements[0]);
    auto condition = getNode<BooleanLiteralExpression>(ifStatement->condition);
    REQUIRE(condition->value == true);
    REQUIRE(ifStatement->thenBranch->type == AstNodeType::BlockStatement);
    auto block = getNode<BlockStatement>(ifStatement->thenBranch);
    REQUIRE(block->statements.size() == 1);
    auto printStatement = getNode<PrintStatement>(block->statements[0]);
    auto literal = getNode<NumericLiteralExpression>(printStatement->expr);
    REQUIRE(literal->value == 1);
}

TEST_CASE("Parses if without block", "[parser][controlFlow]") {
    std::wstring source = L"ako (istina) ispisi 1; ispisi 2;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 2);
    auto ifStatement = getNode<IfStatement>(program->statements[0]);
    auto condition = getNode<BooleanLiteralExpression>(ifStatement->condition);
    REQUIRE(condition->value == true);
    REQUIRE(ifStatement->thenBranch->type == AstNodeType::PrintStatement);
    auto printStatement = getNode<PrintStatement>(ifStatement->thenBranch);
    auto literal = getNode<NumericLiteralExpression>(printStatement->expr);
    REQUIRE(literal->value == 1);
    REQUIRE(program->statements[1]->type == AstNodeType::PrintStatement);
    printStatement = getNode<PrintStatement>(program->statements[1]);
    literal = getNode<NumericLiteralExpression>(printStatement->expr);
    REQUIRE(literal->value == 2);
}

TEST_CASE("Parses if with else", "[parser][controlFlow]") {
    std::wstring source = L"ako (istina) ispisi 1; inace ispisi 2;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto ifStatement = getNode<IfStatement>(program->statements[0]);
    auto condition = getNode<BooleanLiteralExpression>(ifStatement->condition);
    REQUIRE(condition->value == true);
    REQUIRE(ifStatement->thenBranch->type == AstNodeType::PrintStatement);
    auto printStatement = getNode<PrintStatement>(ifStatement->thenBranch);
    auto literal = getNode<NumericLiteralExpression>(printStatement->expr);
    REQUIRE(literal->value == 1);
    REQUIRE(ifStatement->elseBranch->type == AstNodeType::PrintStatement);
    printStatement = getNode<PrintStatement>(ifStatement->elseBranch);
    literal = getNode<NumericLiteralExpression>(printStatement->expr);
    REQUIRE(literal->value == 2);
}

TEST_CASE("Parses if-else-if-else statement", "[parser][controlFlow]"){
    std::wstring source = L"ako (istina) ispisi 1; inace ako (istina) ispisi 2; inace ispisi 3;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto ifStatement = getNode<IfStatement>(program->statements[0]);
    auto condition = getNode<BooleanLiteralExpression>(ifStatement->condition);
    REQUIRE(condition->value == true);
    REQUIRE(ifStatement->thenBranch->type == AstNodeType::PrintStatement);
    auto printStatement = getNode<PrintStatement>(ifStatement->thenBranch);
    auto literal = getNode<NumericLiteralExpression>(printStatement->expr);
    REQUIRE(literal->value == 1);
    REQUIRE(ifStatement->elseBranch->type == AstNodeType::IfStatement);
    auto elseIfStatement = getNode<IfStatement>(ifStatement->elseBranch);
    condition = getNode<BooleanLiteralExpression>(elseIfStatement->condition);
    REQUIRE(condition->value == true);
    REQUIRE(elseIfStatement->thenBranch->type == AstNodeType::PrintStatement);
    printStatement = getNode<PrintStatement>(elseIfStatement->thenBranch);
    literal = getNode<NumericLiteralExpression>(printStatement->expr);
    REQUIRE(literal->value == 2);
    REQUIRE(elseIfStatement->elseBranch->type == AstNodeType::PrintStatement);
    printStatement = getNode<PrintStatement>(elseIfStatement->elseBranch);
    literal = getNode<NumericLiteralExpression>(printStatement->expr);
    REQUIRE(literal->value == 3);
}

TEST_CASE("Ties else to nearest if statement", "[parser][controlFlow]"){
    std::wstring source = L"ako (istina) ako(istina) ispisi 1; inace ispisi 2;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto ifStatement = getNode<IfStatement>(program->statements[0]);
    auto condition = getNode<BooleanLiteralExpression>(ifStatement->condition);
    REQUIRE(condition->value == true);
    REQUIRE(ifStatement->thenBranch->type == AstNodeType::IfStatement);
    auto innerIfStatement = getNode<IfStatement>(ifStatement->thenBranch);
    condition = getNode<BooleanLiteralExpression>(innerIfStatement->condition);
    REQUIRE(condition->value == true);
    REQUIRE(innerIfStatement->thenBranch->type == AstNodeType::PrintStatement);
    auto printStatement = getNode<PrintStatement>(innerIfStatement->thenBranch);
    auto literal = getNode<NumericLiteralExpression>(printStatement->expr);
    REQUIRE(literal->value == 1);
    REQUIRE(innerIfStatement->elseBranch->type == AstNodeType::PrintStatement);
    printStatement = getNode<PrintStatement>(innerIfStatement->elseBranch);
    literal = getNode<NumericLiteralExpression>(printStatement->expr);
    REQUIRE(literal->value == 2);
}

TEST_CASE("Throws error if no condition in if statement", "[parser][controlFlow]") {
    std::wstring source = L"ako (){ ispisi 1; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws error on no beginning parenthesis", "[parser][controlFlow]") {
    std::wstring source = L"ako istina) { ispisi 1; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws error on no closing parenthesis", "[parser][controlFlow]") {
    std::wstring source = L"ako (istina { ispisi 1; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws error on no then branch", "[parser][controlFlow]") {
    std::wstring source = L"ako (istina) ";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}
