//
// Created by kemal on 2/5/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Parser.h"
#include "Lexer.h"
#include "Program.h"

TEST_CASE("Parses empty program", "[parser]") {
    std::wstring source = L"";
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();

    REQUIRE(program->statements.empty());
}