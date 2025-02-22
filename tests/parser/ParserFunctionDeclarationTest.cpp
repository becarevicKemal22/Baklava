//
// Created by kemal on 3/17/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Program.h"
#include "FunctionDeclarationStatement.h"
#include "../TestHelpers.h"
#include "ExpectedXBeforeY.h"
#include "ReturnStatement.h"
#include "NumericLiteralExpression.h"

TEST_CASE("Parses function declaration with no parameters", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo() { }";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto functionDeclaration = dynamic_cast<FunctionDeclarationStatement*>(program->statements[0]);
    REQUIRE(functionDeclaration);
    REQUIRE(functionDeclaration->name->value == L"foo");
    REQUIRE(functionDeclaration->parameters.size() == 0);
    REQUIRE(functionDeclaration->body.empty());
}

TEST_CASE("Parses function declaration with parameters", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo(a, b, c) { }";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto functionDeclaration = dynamic_cast<FunctionDeclarationStatement*>(program->statements[0]);
    REQUIRE(functionDeclaration);
    REQUIRE(functionDeclaration->name->value == L"foo");
    REQUIRE(functionDeclaration->parameters.size() == 3);
    REQUIRE(functionDeclaration->parameters[0]->value == L"a");
    REQUIRE(functionDeclaration->parameters[1]->value == L"b");
    REQUIRE(functionDeclaration->parameters[2]->value == L"c");
    REQUIRE(functionDeclaration->body.empty());
}

TEST_CASE("Parses function declaration with body", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo() { var x = 10; }";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto functionDeclaration = dynamic_cast<FunctionDeclarationStatement*>(program->statements[0]);
    REQUIRE(functionDeclaration);
    REQUIRE(functionDeclaration->name->value == L"foo");
    REQUIRE(functionDeclaration->parameters.empty());
    REQUIRE(functionDeclaration->body.size() == 1);
    REQUIRE(functionDeclaration->body[0]->type == AstNodeType::VarDeclarationStatement);
}

TEST_CASE("Parses function with return statement", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo() { vrati 5; }";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto functionDeclaration = dynamic_cast<FunctionDeclarationStatement*>(program->statements[0]);
    REQUIRE(functionDeclaration);
    REQUIRE(functionDeclaration->name->value == L"foo");
    REQUIRE(functionDeclaration->parameters.empty());
    REQUIRE(functionDeclaration->body.size() == 1);
    REQUIRE(functionDeclaration->body[0]->type == AstNodeType::ReturnStatement);
    auto returnStatement = getNode<ReturnStatement>(functionDeclaration->body[0]);
    REQUIRE(returnStatement->value->type == AstNodeType::NumericLiteralExpression);
    auto numericLiteral = getNode<NumericLiteralExpression>(returnStatement->value);
    REQUIRE(numericLiteral->value == 5);
    REQUIRE(returnStatement->keyword->value == L"vrati");
}

TEST_CASE("Parses function with empty return statement", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo() { vrati; }";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto functionDeclaration = dynamic_cast<FunctionDeclarationStatement*>(program->statements[0]);
    REQUIRE(functionDeclaration);
    REQUIRE(functionDeclaration->name->value == L"foo");
    REQUIRE(functionDeclaration->parameters.empty());
    REQUIRE(functionDeclaration->body.size() == 1);
    REQUIRE(functionDeclaration->body[0]->type == AstNodeType::ReturnStatement);
    auto returnStatement = getNode<ReturnStatement>(functionDeclaration->body[0]);
    REQUIRE(returnStatement->value == nullptr);
    REQUIRE(returnStatement->keyword->value == L"vrati");
}

TEST_CASE("Throws on function declaration with no name", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija () { }";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), ExpectedXBeforeY);
}

TEST_CASE("Throws on function declaration with no opening parenthesis", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo) { }";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), ExpectedXBeforeY);
}

TEST_CASE("Throws on function declaration with no closing parenthesis", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo( { }";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), ExpectedXBeforeY);
}

TEST_CASE("Throws on function with no body", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo()";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), ExpectedXBeforeY);
}

TEST_CASE("Throws on function parameters not separated by comma", "[parser][function][functionDeclaration]") {
    std::wstring source = L"funkcija foo(a b c) { }";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), ExpectedXBeforeY);
}