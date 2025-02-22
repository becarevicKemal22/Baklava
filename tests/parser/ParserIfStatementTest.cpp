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
    std::wstring source = L"ako tacno onda { ispiši 1; }";
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
    std::wstring source = L"ako tačno onda ispiši 1; ispiši 2;";
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
    std::wstring source = L"ako tačno onda ispiši 1; inače ispiši 2;";
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
    std::wstring source = L"ako tačno onda ispiši 1; inače ako tačno onda ispiši 2; inače ispiši 3;";
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
    std::wstring source = L"ako tačno onda ako tačno onda ispiši 1; inače ispiši 2;";
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

TEST_CASE("Allows 'is' keyword when to help with readability", "[parser][controlFlow]") {
    std::wstring source = L"ako je a == tačno onda ispiši 1; inače ako je a == 5 onda ispiši 2;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();
    REQUIRE(program->statements.size() == 1);
    auto ifStatement = getNode<IfStatement>(program->statements[0]);
    auto condition = getNode<BinaryExpression>(ifStatement->condition);
    REQUIRE(condition->op->type == TokenType::DoubleEqual);
    REQUIRE(condition->left->type == AstNodeType::VariableExpression);
    REQUIRE(condition->right->type == AstNodeType::BooleanLiteralExpression);
    REQUIRE(ifStatement->thenBranch->type == AstNodeType::PrintStatement);
    REQUIRE(ifStatement->elseBranch->type == AstNodeType::IfStatement);
    auto elseIfStatement = getNode<IfStatement>(ifStatement->elseBranch);
    condition = getNode<BinaryExpression>(elseIfStatement->condition);
    REQUIRE(condition->op->type == TokenType::DoubleEqual);
    REQUIRE(condition->left->type == AstNodeType::VariableExpression);
    REQUIRE(condition->right->type == AstNodeType::NumericLiteralExpression);
    REQUIRE(elseIfStatement->thenBranch->type == AstNodeType::PrintStatement);
}

TEST_CASE("Assumes equality operator when 'je' is used (boolean literal)", "[parser][controlFlow]") {
    std::wstring source = L"ako je a tačno onda ispiši 1;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();
    REQUIRE(program->statements.size() == 1);
    auto ifStatement = getNode<IfStatement>(program->statements[0]);
    auto condition = getNode<BinaryExpression>(ifStatement->condition);
    REQUIRE(condition->op->type == TokenType::DoubleEqual);
    REQUIRE(condition->left->type == AstNodeType::VariableExpression);
    REQUIRE(condition->right->type == AstNodeType::BooleanLiteralExpression);
    REQUIRE(ifStatement->thenBranch->type == AstNodeType::PrintStatement);
}

TEST_CASE("Does not assume equality operator when 'je' is used (numeric literal)", "[parser][controlFlow]") {
    std::wstring source = L"ako je a + 2 5 onda ispiši 1;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Assumes equality operator when 'je' is used (null literal)", "[parser][controlFlow]") {
    std::wstring source = L"ako je poziv() null onda ispiši 1;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();
    REQUIRE(program->statements.size() == 1);
    auto ifStatement = getNode<IfStatement>(program->statements[0]);
    auto condition = getNode<BinaryExpression>(ifStatement->condition);
    REQUIRE(condition->op->type == TokenType::DoubleEqual);
    REQUIRE(condition->left->type == AstNodeType::CallExpression);
    REQUIRE(condition->right->type == AstNodeType::NullLiteralExpression);
    REQUIRE(ifStatement->thenBranch->type == AstNodeType::PrintStatement);
}

TEST_CASE("Allows omition of 'onda' when then branch is block", "[parser][controlFlow]") {
    std::wstring source = L"ako tačno { ispiši 1; }";
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

TEST_CASE("Allows omition of 'onda' when then branch is block (shorthand if)", "[parser][controlFlow]") {
    std::wstring source = L"ako je tačno {ispisi 1;}";
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

TEST_CASE("Allows both 'onda' and block", "[parser][controlFlow]") {
    std::wstring source = L"ako tačno onda { ispiši 1; }";
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

TEST_CASE("Requires onda when no block is used", "[parser][controlFlow]") {
    std::wstring source = L"ako tačno ispiši 1;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Requires onda when no block is used (shorthand if)", "[parser][controlFlow]") {
    std::wstring source = L"ako je a tačno ispiši 1;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws error if no condition in if statement", "[parser][controlFlow]") {
    std::wstring source = L"ako onda{ ispiši 1; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}
// No longer necessary, now then can be omitted when using a block
//TEST_CASE("Throws error on missing 'then'", "[parser][controlFlow]") {
//    std::wstring source = L"ako tačno { ispiši 1; }";
//    Lexer lexer(source);
//    lexer.tokenize();
//    Parser parser(lexer.tokens);
//    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
//}

//TEST_CASE("Throws error on no closing parenthesis", "[parser][controlFlow]") {
//    std::wstring source = L"ako (tačno { ispiši 1; }";
//    Lexer lexer(source);
//    lexer.tokenize();
//    Parser parser(lexer.tokens);
//    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
//}

//TEST_CASE("Throws error on no then branch", "[parser][controlFlow]") {
//    std::wstring source = L"ako (tačno) ";
//    Lexer lexer(source);
//    lexer.tokenize();
//    Parser parser(lexer.tokens);
//    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
//}
