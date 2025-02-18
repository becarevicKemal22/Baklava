//
// Created by kemal on 18-Feb-25.
//

#include <catch2/catch_test_macros.hpp>

#include "Program.h"
#include "FunctionDeclarationStatement.h"
#include "../TestHelpers.h"
#include "ExpectedXBeforeY.h"
#include "ReturnStatement.h"
#include "NumericLiteralExpression.h"
#include "UnaryExpression.h"
#include "BooleanLiteralExpression.h"
#include "StringLiteralExpression.h"
#include "NullLiteralExpression.h"
#include "InvalidDefaultParameterValue.h"
#include "InvalidDefaultParameterPosition.h"


TEST_CASE("Parses function declaration with one default param", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo(a = 5) { }";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto functionDeclaration = dynamic_cast<FunctionDeclarationStatement*>(program->statements[0]);
    REQUIRE(functionDeclaration);
    REQUIRE(functionDeclaration->name->value == L"foo");
    REQUIRE(functionDeclaration->parameters.size() == 1);
    REQUIRE(functionDeclaration->parameters[0]->value == L"a");
    REQUIRE(functionDeclaration->defaultParameters.size() == 1);
    REQUIRE(functionDeclaration->defaultParameters[0]->type == AstNodeType::NumericLiteralExpression);
    REQUIRE(dynamic_cast<NumericLiteralExpression*>(functionDeclaration->defaultParameters[0])->value == 5);
    REQUIRE(functionDeclaration->body.empty());
}

TEST_CASE("Parses function declaration with 1 required and 1 default param", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo(a, b = 5) { }";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto functionDeclaration = dynamic_cast<FunctionDeclarationStatement*>(program->statements[0]);
    REQUIRE(functionDeclaration);
    REQUIRE(functionDeclaration->name->value == L"foo");
    REQUIRE(functionDeclaration->parameters.size() == 2);
    REQUIRE(functionDeclaration->parameters[0]->value == L"a");
    REQUIRE(functionDeclaration->parameters[1]->value == L"b");
    REQUIRE(functionDeclaration->defaultParameters.size() == 1);
    REQUIRE(functionDeclaration->defaultParameters[0]->type == AstNodeType::NumericLiteralExpression);
    REQUIRE(dynamic_cast<NumericLiteralExpression*>(functionDeclaration->defaultParameters[0])->value == 5);
    REQUIRE(functionDeclaration->body.empty());
}

TEST_CASE("Parses function declaration with 2 default parameters", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo(a = 5, b = 10) { }";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto functionDeclaration = dynamic_cast<FunctionDeclarationStatement*>(program->statements[0]);
    REQUIRE(functionDeclaration);
    REQUIRE(functionDeclaration->name->value == L"foo");
    REQUIRE(functionDeclaration->parameters.size() == 2);
    REQUIRE(functionDeclaration->parameters[0]->value == L"a");
    REQUIRE(functionDeclaration->parameters[1]->value == L"b");
    REQUIRE(functionDeclaration->defaultParameters.size() == 2);
    REQUIRE(functionDeclaration->defaultParameters[0]->type == AstNodeType::NumericLiteralExpression);
    REQUIRE(dynamic_cast<NumericLiteralExpression*>(functionDeclaration->defaultParameters[0])->value == 5);
    REQUIRE(functionDeclaration->defaultParameters[1]->type == AstNodeType::NumericLiteralExpression);
    REQUIRE(dynamic_cast<NumericLiteralExpression*>(functionDeclaration->defaultParameters[1])->value == 10);
    REQUIRE(functionDeclaration->body.empty());
}

TEST_CASE("Correctly passes negative number (unaryexpr literal exception)", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo(a = -5) { }";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto functionDeclaration = dynamic_cast<FunctionDeclarationStatement*>(program->statements[0]);
    REQUIRE(functionDeclaration);
    REQUIRE(functionDeclaration->name->value == L"foo");
    REQUIRE(functionDeclaration->parameters.size() == 1);
    REQUIRE(functionDeclaration->parameters[0]->value == L"a");
    REQUIRE(functionDeclaration->defaultParameters.size() == 1);
    REQUIRE(functionDeclaration->defaultParameters[0]->type == AstNodeType::UnaryExpression);
    REQUIRE(dynamic_cast<UnaryExpression*>(functionDeclaration->defaultParameters[0])->op->type == TokenType::Minus);
    REQUIRE(dynamic_cast<NumericLiteralExpression*>(dynamic_cast<UnaryExpression*>(functionDeclaration->defaultParameters[0])->expr)->value == 5);
    REQUIRE(functionDeclaration->body.empty());
}

TEST_CASE("Correctly parses all types of literals", "[parser][function][functionDeclaration]"){
    std::wstring source = L"funkcija foo(a, b = 5.5, c = null, d = tacno, e = netacno, f = \"mojString\", g = -3.14) { }";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto functionDeclaration = dynamic_cast<FunctionDeclarationStatement*>(program->statements[0]);
    REQUIRE(functionDeclaration);
    REQUIRE(functionDeclaration->name->value == L"foo");
    REQUIRE(functionDeclaration->parameters.size() == 7);
    REQUIRE(functionDeclaration->parameters[0]->value == L"a");
    REQUIRE(functionDeclaration->parameters[1]->value == L"b");
    REQUIRE(functionDeclaration->parameters[2]->value == L"c");
    REQUIRE(functionDeclaration->parameters[3]->value == L"d");
    REQUIRE(functionDeclaration->parameters[4]->value == L"e");
    REQUIRE(functionDeclaration->parameters[5]->value == L"f");
    REQUIRE(functionDeclaration->parameters[6]->value == L"g");
    REQUIRE(functionDeclaration->defaultParameters.size() == 6);
    REQUIRE(functionDeclaration->defaultParameters[0]->type == AstNodeType::NumericLiteralExpression);
    REQUIRE(dynamic_cast<NumericLiteralExpression*>(functionDeclaration->defaultParameters[0])->value == 5.5);
    REQUIRE(functionDeclaration->defaultParameters[1]->type == AstNodeType::NullLiteralExpression);
    REQUIRE(dynamic_cast<NullLiteralExpression*>(functionDeclaration->defaultParameters[1]));
    REQUIRE(functionDeclaration->defaultParameters[2]->type == AstNodeType::BooleanLiteralExpression);
    REQUIRE(dynamic_cast<BooleanLiteralExpression*>(functionDeclaration->defaultParameters[2])->value == true);
    REQUIRE(functionDeclaration->defaultParameters[3]->type == AstNodeType::BooleanLiteralExpression);
    REQUIRE(dynamic_cast<BooleanLiteralExpression*>(functionDeclaration->defaultParameters[3])->value == false);
    REQUIRE(functionDeclaration->defaultParameters[4]->type == AstNodeType::StringLiteralExpression);
    REQUIRE(dynamic_cast<StringLiteralExpression*>(functionDeclaration->defaultParameters[4])->value == L"mojString");
    REQUIRE(functionDeclaration->defaultParameters[5]->type == AstNodeType::UnaryExpression);
    REQUIRE(dynamic_cast<UnaryExpression*>(functionDeclaration->defaultParameters[5])->op->type == TokenType::Minus);
    REQUIRE(dynamic_cast<NumericLiteralExpression*>(dynamic_cast<UnaryExpression*>(functionDeclaration->defaultParameters[5])->expr)->value == 3.14);
    REQUIRE(functionDeclaration->body.empty());
}

TEST_CASE("Throws on non-literal default parameter", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo(a = b) { }";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), InvalidDefaultParameterValue);
}

TEST_CASE("Throws on default parameter before required parameter", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo(a = 5, b) { }";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), InvalidDefaultParameterPosition);
}

TEST_CASE("Throws on required parameter after default parameters", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo(a = 5, b = 10, c, d = 15) { }";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), InvalidDefaultParameterPosition);
}

TEST_CASE("Throws on function call as default parameter", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo(a = bar()) { }";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), InvalidDefaultParameterValue);
}