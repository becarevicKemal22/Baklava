//
// Created by kemal on 3/17/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Program.h"
#include "Token.h"
#include "CallExpression.h"
#include "../TestHelpers.h"
#include "ExpectedXBeforeY.h"

TEST_CASE("Parses call expression with no arguments", "[parser][function][callExpression]") {
    std::wstring source = L"foo();";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto callExpression = getNode<CallExpression>(program->statements[0]);
    REQUIRE(callExpression->callee->type == AstNodeType::VariableExpression);
    REQUIRE(callExpression->arguments.size() == 0);
}

TEST_CASE("Parses call expression with arguments", "[parser][function][callExpression]") {
    std::wstring source = L"foo(1, 2, variable);";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto callExpression = getNode<CallExpression>(program->statements[0]);
    REQUIRE(callExpression->callee->type == AstNodeType::VariableExpression);
    REQUIRE(callExpression->arguments.size() == 3);
    REQUIRE(callExpression->arguments[0]->type == AstNodeType::NumericLiteralExpression);
    REQUIRE(callExpression->arguments[1]->type == AstNodeType::NumericLiteralExpression);
    REQUIRE(callExpression->arguments[2]->type == AstNodeType::VariableExpression);
}

TEST_CASE("Parses chained function calls", "[parser][function][callExpression]") {
    std::wstring source = L"foo(variable)(3, 4);";
    std::unique_ptr<Program> program = parseSourceNoResolver(source);

    REQUIRE(program->statements.size() == 1);
    auto callExpression = getNode<CallExpression>(program->statements[0]);
    REQUIRE(callExpression->arguments.size() == 2);
    REQUIRE(callExpression->arguments[0]->type == AstNodeType::NumericLiteralExpression);
    REQUIRE(callExpression->arguments[1]->type == AstNodeType::NumericLiteralExpression);

    REQUIRE(callExpression->callee->type == AstNodeType::CallExpression);
    auto callee = getNode<CallExpression>(callExpression->callee);
    REQUIRE(callee->callee->type == AstNodeType::VariableExpression);
    REQUIRE(callee->arguments.size() == 1);
    REQUIRE(callee->arguments[0]->type == AstNodeType::VariableExpression);
}

TEST_CASE("Throws on call expression with no closing parenthesis", "[parser][function][callExpression]") {
    std::wstring source = L"foo(";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), ExpectedXBeforeY);
}

TEST_CASE("Throws on call expression with no opening parenthesis", "[parser][function][callExpression]") {
    std::wstring source = L"foo)";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), ExpectedXBeforeY);
}

TEST_CASE("Throws on call expression with no closing parenthesis and arguments", "[parser][function][callExpression]") {
    std::wstring source = L"foo(1, 2";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), ExpectedXBeforeY);
}

TEST_CASE("Throws on call expression with no opening parenthesis and arguments", "[parser][function][callExpression]") {
    std::wstring source = L"foo 1, 2)";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), ExpectedXBeforeY);
}

TEST_CASE("Throws when arguments are not comma-separated", "[parser][function][callExpression]") {
    std::wstring source = L"foo(1 2)";
    REQUIRE_THROWS_AS(parseSourceNoResolver(source), ExpectedXBeforeY);
}
