//
// Created by kemal on 3/14/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Parser.h"
#include "Lexer.h"
#include "Program.h"
#include "../TestHelpers.h"
#include "VariableExpression.h"

TEST_CASE("Parses variable expression", "[parser][variableExpression]"){
    std::wstring source = L"myVar;";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    std::unique_ptr<Program> program = parser.parse();
    REQUIRE(program->statements.size() == 1);

    auto variableExpression = getNode<VariableExpression>(program->statements[0]);
    REQUIRE(variableExpression->name->value == L"myVar");
}
