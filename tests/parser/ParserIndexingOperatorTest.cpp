//
// Created by kemal on 5/3/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Program.h"
#include "Token.h"
#include "../TestHelpers.h"
#include "ExpectedXBeforeY.h"
#include "IndexingExpression.h"
#include "CallExpression.h"
#include "VariableExpression.h"

TEST_CASE("Parses indexing operator with number parameter", "[parser][array][indexing]") {
    std::wstring source = L"foo[1];";
    std::unique_ptr<Program> program = parseSource(source);

    REQUIRE(program->statements.size() == 1);
    auto indexingExpression = getNode<IndexingExpression>(program->statements[0]);
    REQUIRE(indexingExpression->index->type == AstNodeType::NumericLiteralExpression);
}

TEST_CASE("Parses indexing operator with variable parameter", "[parser][array][indexing]") {
    std::wstring source = L"foo[variable];";
    std::unique_ptr<Program> program = parseSource(source);

    REQUIRE(program->statements.size() == 1);
    auto indexingExpression = getNode<IndexingExpression>(program->statements[0]);
    REQUIRE(indexingExpression->index->type == AstNodeType::VariableExpression);
}

TEST_CASE("Parses nested indexing operator", "[parser][array][indexing]") {
    std::wstring source = L"foo[1][variable];";
    std::unique_ptr<Program> program = parseSource(source);

    REQUIRE(program->statements.size() == 1);
    auto indexingExpression = getNode<IndexingExpression>(program->statements[0]);
    REQUIRE(indexingExpression->index->type == AstNodeType::VariableExpression);

    auto nestedIndexingExpression = getNode<IndexingExpression>(indexingExpression->left);
    REQUIRE(nestedIndexingExpression->index->type == AstNodeType::NumericLiteralExpression);
}

TEST_CASE("Parses function call followed by indexing operator", "[parser][array][indexing]") {
    std::wstring source = L"foo()[1];";
    std::unique_ptr<Program> program = parseSource(source);

    REQUIRE(program->statements.size() == 1);
    auto indexingExpression = getNode<IndexingExpression>(program->statements[0]);
    auto functionCall = getNode<CallExpression>(indexingExpression->left);
    REQUIRE(functionCall->arguments.empty());
    REQUIRE(functionCall->callee->type == AstNodeType::VariableExpression);
    REQUIRE(getNode<VariableExpression>(functionCall->callee)->name->value == L"foo");
    REQUIRE(indexingExpression->index->type == AstNodeType::NumericLiteralExpression);
}

TEST_CASE("Throws on indexing operator with no closing bracket", "[parser][array][indexing]") {
    std::wstring source = L"foo[";
    REQUIRE_THROWS_AS(parseSource(source), ExpectedXBeforeY);
}

TEST_CASE("Throws on indexing operator with no opening bracket", "[parser][array][indexing]") {
    std::wstring source = L"foo]";
    REQUIRE_THROWS_AS(parseSource(source), ExpectedXBeforeY);
}

TEST_CASE("Throws on indexing operator with no index", "[parser][array][indexing]") {
    std::wstring source = L"foo[]";
    REQUIRE_THROWS_AS(parseSource(source), ExpectedXBeforeY);
}