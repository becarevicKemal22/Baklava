
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
#include "PrintStatement.h"
#include "UnaryExpression.h"
#include "../TestHelpers.h"

TEST_CASE("Parses empty for loop", "[parser][controlFlow]") {
    std::wstring source = L"za svako j od 0 do 10 { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"j");
    auto initialValue = getNode<NumericLiteralExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->value == 0);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BinaryExpression>(forLoop->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"j");
    auto right = getNode<NumericLiteralExpression>(condition->right);
    REQUIRE(right->value == 10);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 2);
    auto loopBlockStatement = getNode<BlockStatement>(loopBlock->statements[0]);
    REQUIRE(loopBlockStatement->statements.size() == 0);
    auto increment = getNode<AssignmentExpression>(loopBlock->statements[1]);
    REQUIRE(increment->name->value == L"j");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"j");
    REQUIRE(getNode<NumericLiteralExpression>(incrementValue->right)->value == 1);
}

TEST_CASE("Parses for loop with multiple statements", "[parser][controlFlow]") {
    std::wstring source = L"za svako j od 0 do 10 { var x = 0; }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"j");
    auto initialValue = getNode<NumericLiteralExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->value == 0);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BinaryExpression>(forLoop->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"j");
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
    REQUIRE(increment->name->value == L"j");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"j");
    REQUIRE(getNode<NumericLiteralExpression>(incrementValue->right)->value == 1);

}

TEST_CASE("Works with every grammatical voice 1", "[parser][controlFlow]") {
    std::wstring source = L"za svaki broj od 0 do 100 { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"broj");
    auto initialValue = getNode<NumericLiteralExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->value == 0);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BinaryExpression>(forLoop->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"broj");
    auto right = getNode<NumericLiteralExpression>(condition->right);
    REQUIRE(right->value == 100);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 2);
    auto increment = getNode<AssignmentExpression>(loopBlock->statements[1]);
    REQUIRE(increment->name->value == L"broj");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"broj");
    REQUIRE(getNode<NumericLiteralExpression>(incrementValue->right)->value == 1);
}

TEST_CASE("Works with every grammatical voice 2", "[parser][controlFlow]") {
    std::wstring source = L"za svaku vrijednost od 0 do 100 { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"vrijednost");
    auto initialValue = getNode<NumericLiteralExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->value == 0);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BinaryExpression>(forLoop->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"vrijednost");
    auto right = getNode<NumericLiteralExpression>(condition->right);
    REQUIRE(right->value == 100);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 2);
    auto increment = getNode<AssignmentExpression>(loopBlock->statements[1]);
    REQUIRE(increment->name->value == L"vrijednost");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"vrijednost");
    REQUIRE(getNode<NumericLiteralExpression>(incrementValue->right)->value == 1);
}

TEST_CASE("Parses negative limits", "[parser][controlFlow]"){
    std::wstring source = L"za svako j od -10 do -1 { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"j");
    auto initialValue = getNode<UnaryExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->op->type == TokenType::Minus);
    REQUIRE(getNode<NumericLiteralExpression>(initialValue->expr)->value == 10);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BinaryExpression>(forLoop->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"j");
    auto right = getNode<UnaryExpression>(condition->right);
    REQUIRE(right->op->type == TokenType::Minus);
    REQUIRE(getNode<NumericLiteralExpression>(right->expr)->value == 1);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 2);
    auto loopBlockStatement = getNode<BlockStatement>(loopBlock->statements[0]);
    REQUIRE(loopBlockStatement->statements.size() == 0);
    auto increment = getNode<AssignmentExpression>(loopBlock->statements[1]);
    REQUIRE(increment->name->value == L"j");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"j");
    REQUIRE(getNode<NumericLiteralExpression>(incrementValue->right)->value == 1);
}

TEST_CASE("Allows use of repeat keyword along with block statement", "[parser][controlFlow]") {
    std::wstring source = L"za svako j od 0 do 100 ponavljaj {}";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"j");
    auto initialValue = getNode<NumericLiteralExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->value == 0);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BinaryExpression>(forLoop->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"j");
    auto right = getNode<NumericLiteralExpression>(condition->right);
    REQUIRE(right->value == 100);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 2);
    auto userCodeBlock = getNode<BlockStatement>(loopBlock->statements[0]);
    REQUIRE(userCodeBlock->statements.size() == 0);
    auto increment = getNode<AssignmentExpression>(loopBlock->statements[1]);
    REQUIRE(increment->name->value == L"j");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"j");
    REQUIRE(getNode<NumericLiteralExpression>(incrementValue->right)->value == 1);
}

TEST_CASE("Allows omition of block stmt in exchange for single stmt when repeat is used", "[parser][controlFlow]") {
    std::wstring source = L"za svako j od 0 do 100 ponavljaj ispisi j;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"j");
    auto initialValue = getNode<NumericLiteralExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->value == 0);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BinaryExpression>(forLoop->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"j");
    auto right = getNode<NumericLiteralExpression>(condition->right);
    REQUIRE(right->value == 100);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 2);
    auto printStatement = getNode<PrintStatement>(loopBlock->statements[0]);
    auto printExpression = getNode<VariableExpression>(printStatement->expr);
    REQUIRE(printExpression->name->value == L"j");
    auto increment = getNode<AssignmentExpression>(loopBlock->statements[1]);
    REQUIRE(increment->name->value == L"j");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"j");
    REQUIRE(getNode<NumericLiteralExpression>(incrementValue->right)->value == 1);
}

TEST_CASE("Parses step / increment", "[parser][controlFlow]") {
    std::wstring source = L"za svako j od 0 do 100 korakom 2 { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"j");
    auto initialValue = getNode<NumericLiteralExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->value == 0);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BinaryExpression>(forLoop->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"j");
    auto right = getNode<NumericLiteralExpression>(condition->right);
    REQUIRE(right->value == 100);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 2);
    auto increment = getNode<AssignmentExpression>(loopBlock->statements[1]);
    REQUIRE(increment->name->value == L"j");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"j");
    REQUIRE(getNode<NumericLiteralExpression>(incrementValue->right)->value == 2);
}

// Note on this test: it is obvious that the comparison operator for the limit should be 'greater', but this will be checked in the
// interpreter, as the user can pass anything to the 'korakom' keyword. This means that if it's not strictly a numeric
// literal expression, the parser has no way of knowing what operator it should use.
TEST_CASE("Parses step / increment with negative value", "[parser][controlFlow]") {
    std::wstring source = L"za svako j od 0 do 100 korakom -2 { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.size() == 1);
    auto mainBlock = getNode<BlockStatement>(program->statements[0]);
    REQUIRE(mainBlock->statements.size() == 2);
    auto initializerDeclaration = getNode<VarDeclarationStatement>(mainBlock->statements[0]);
    REQUIRE(initializerDeclaration->name->value == L"j");
    auto initialValue = getNode<NumericLiteralExpression>(initializerDeclaration->initializer);
    REQUIRE(initialValue->value == 0);
    auto forLoop = getNode<WhileStatement>(mainBlock->statements[1]);
    auto condition = getNode<BinaryExpression>(forLoop->condition);
    REQUIRE(condition->op->type == TokenType::Less);
    auto left = getNode<VariableExpression>(condition->left);
    REQUIRE(left->name->value == L"j");
    auto right = getNode<NumericLiteralExpression>(condition->right);
    REQUIRE(right->value == 100);
    auto loopBlock = getNode<BlockStatement>(forLoop->body);
    REQUIRE(loopBlock->statements.size() == 2);
    auto increment = getNode<AssignmentExpression>(loopBlock->statements[1]);
    REQUIRE(increment->name->value == L"j");
    auto incrementValue = getNode<BinaryExpression>(increment->value);
    REQUIRE(incrementValue->op->type == TokenType::Plus);
    REQUIRE(getNode<VariableExpression>(incrementValue->left)->name->value == L"j");
    auto unaryMinus = getNode<UnaryExpression>(incrementValue->right);
    REQUIRE(unaryMinus->op->type == TokenType::Minus);
    REQUIRE(getNode<NumericLiteralExpression>(unaryMinus->expr)->value == 2);
}

TEST_CASE("Throws on no step / increment after using 'korakom'", "[parser][controlFlow]") {
    std::wstring source = L"za svako j od 0 do 100 korakom { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Requires identifier as 'initializer' 1", "[parser][controlFlow]") {
    std::wstring source = L"za svako 0 od 0 do 100 { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Requires identifier as 'initializer' 2", "[parser][controlFlow]") {
    std::wstring source = L"za svako funk() 0 od 0 do 100 { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on no body", "[parser][controlFlow]") {
    std::wstring source = L"za svako j od 0 do 100";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on no to-from", "[parser][controlFlow]") {
    std::wstring source = L"za svako { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on missing everything", "[parser][controlFlow]") {
    std::wstring source = L"za { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on missing 'to'", "[parser][controlFlow]") {
    std::wstring source = L"za svako j od 0 { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on missing 'from' expression", "[parser][controlFlow]") {
    std::wstring source = L"za svako j od do 100 { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws on missing 'to' expression", "[parser][controlFlow]") {
    std::wstring source = L"za svako j od 0 do { }";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}

TEST_CASE("Throws when missing single statement when repeat keyword is used", "[parser][controlFlow]") {
    std::wstring source = L"za svako j od 0 do 100 ponavljaj";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    REQUIRE_THROWS_AS(parser.parse(), ExpectedXBeforeY);
}