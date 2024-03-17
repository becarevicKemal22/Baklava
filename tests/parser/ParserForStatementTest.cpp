
//
// Created by kemal on 3/16/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Parser.h"
#include "Lexer.h"
#include "WhileStatement.h"
#include "BlockStatement.h"
#include "Program.h"
#include "VarDeclarationStatement.h"
#include "AssignmentExpression.h"
#include "NumericLiteralExpression.h"
#include "BinaryExpression.h"
#include "VariableExpression.h"
#include "BooleanLiteralExpression.h"
#include "../TestHelpers.h"

TEST_CASE("Parses empty for loop", "[parser][controlFlow]") {
    std::wstring source = L"za (var i = 0; i < 10; i = i + 1) { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"i");
    auto initialValue = getNode<NumericLiteralExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->value == 0);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BinaryExpression>(forLoop->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"i");
    auto right = getNode<NumericLiteralExpression>(condition->right);
    REQUIRE(right->value == 10);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 2);
    auto loopBlockStatement = getNode<BlockStatement>(loopBlock->statements[0]);
    REQUIRE(loopBlockStatement->statements.size() == 0);
    auto increment = getNode<AssignmentExpression>(loopBlock->statements[1]);
    REQUIRE(increment->name->value == L"i");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"i");
    REQUIRE(getNode<NumericLiteralExpression>(incrementValue->right)->value == 1);
}

TEST_CASE("Parses for loop with multiple statements", "[parser][controlFlow]") {
    std::wstring source = L"za (var i = 0; i < 10; i = i + 1) { var x = 0; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"i");
    auto initialValue = getNode<NumericLiteralExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->value == 0);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BinaryExpression>(forLoop->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"i");
    auto right = getNode<NumericLiteralExpression>(condition->right);
    REQUIRE(right->value == 10);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 2);
    auto blockStatement = getNode<BlockStatement>(loopBlock->statements[0]);
    REQUIRE(blockStatement->statements.size() == 1);
    auto innerDeclaration = getNode<VarDeclarationStatement>(blockStatement->statements[0]);
    REQUIRE(innerDeclaration->name->value == L"x");
    REQUIRE(getNode<NumericLiteralExpression>(innerDeclaration->initializer)->value == 0);
    auto increment = getNode<AssignmentExpression>(loopBlock->statements[1]);
    REQUIRE(increment->name->value == L"i");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"i");
    REQUIRE(getNode<NumericLiteralExpression>(incrementValue->right)->value == 1);
}

TEST_CASE("Parses for loop without initializer", "[parser][controlFlow]") {
    std::wstring source = L"za (; i < 10; i = i + 1) { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto whileStatement = getNode<WhileStatement>(program->statements[0]);
    auto condition = getNode<BinaryExpression>(whileStatement->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"i");
    auto right = getNode<NumericLiteralExpression>(condition->right);
    REQUIRE(right->value == 10);
    auto loopBlock = getNode<BlockStatement>(whileStatement->body);
    REQUIRE(loopBlock->statements.size() == 2);
    auto loopBlockStatement = getNode<BlockStatement>(loopBlock->statements[0]);
    REQUIRE(loopBlockStatement->statements.size() == 0);
    auto increment = getNode<AssignmentExpression>(loopBlock->statements[1]);
    REQUIRE(increment->name->value == L"i");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"i");
    REQUIRE(getNode<NumericLiteralExpression>(incrementValue->right)->value == 1);
}

TEST_CASE("Parses for loop without condition", "[parser][controlFlow]") {
    std::wstring source = L"za (var i = 0; ; i = i + 1) { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"i");
    auto initialValue = getNode<NumericLiteralExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->value == 0);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BooleanLiteralExpression>(forLoop->condition);
    REQUIRE(condition->value == true);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 2);
    auto increment = getNode<AssignmentExpression>(loopBlock->statements[1]);
    REQUIRE(increment->name->value == L"i");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"i");
    REQUIRE(getNode<NumericLiteralExpression>(incrementValue->right)->value == 1);
}

TEST_CASE("Parses for loop without increment", "[parser][controlFlow]") {
    std::wstring source = L"za (var i = 0; i < 10; ) { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"i");
    auto initialValue = getNode<NumericLiteralExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->value == 0);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BinaryExpression>(forLoop->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"i");
    auto right = getNode<NumericLiteralExpression>(condition->right);
    REQUIRE(right->value == 10);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 0);
}

TEST_CASE("Parses infinite for loop", "[parser][controlFlow]") {
    std::wstring source = L"za (;;) { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto forLoop = getNode<WhileStatement>(program->statements[0]);
    auto condition = getNode<BooleanLiteralExpression>(forLoop->condition);
    REQUIRE(condition->value == true);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 0);
}

TEST_CASE("Throws on no body", "[parser][controlFlow]") {
    std::wstring source = L"za (var i = 0; i < 10; i = i + 1)";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on missing semicolon", "[parser][controlFlow]") {
    std::wstring source = L"za (var i = 0 i < 10; i = i + 1) { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on missing semicolon after condition", "[parser][controlFlow]") {
    std::wstring source = L"za (var i = 0; i < 10 i = i + 1) { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on empty parentheses", "[parser][controlFlow]") {
    std::wstring source = L"za () { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on missing parentheses", "[parser][controlFlow]") {
    std::wstring source = L"za { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on missing closing parentheses", "[parser][controlFlow]") {
    std::wstring source = L"za (var i = 0; i < 10; i = i + 1 { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on missing opening parentheses", "[parser][controlFlow]") {
    std::wstring source = L"za var i = 0; i < 10; i = i + 1) { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}
