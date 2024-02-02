#include <catch2/catch_test_macros.hpp>

#include "Lexer.h"

void checkToken(const Token& token, TokenType type, const std::wstring& value){
    CHECK(token.type == type);
    CHECK(token.value == value);
}

void checkToken(const Token& token, TokenType type, const std::wstring& value, unsigned int line, unsigned int charIndexOnLine){
    checkToken(token, type, value);
    CHECK(token.line == line);
    CHECK(token.offset == charIndexOnLine);
}

TEST_CASE("Adds eof to empty input", "[lexer]"){
    std::wstring source = L"";
    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token> tokens = lexer.tokens;
    REQUIRE(tokens.size() == 1);
    checkToken(tokens[0], TokenType::Eof, L"");
}

TEST_CASE("Correctly tokenizes identifiers and numbers", "[lexer]") {
    std::wstring source = L"24.2 ident 24 ćčđšž";

    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token> tokens = lexer.tokens;
    REQUIRE(tokens.size() == 5);
    checkToken(tokens[0], TokenType::Number, L"24.2");
    checkToken(tokens[1], TokenType::Identifier, L"ident");
    checkToken(tokens[2], TokenType::Number, L"24");
    checkToken(tokens[3], TokenType::Identifier, L"ćčđšž");
}