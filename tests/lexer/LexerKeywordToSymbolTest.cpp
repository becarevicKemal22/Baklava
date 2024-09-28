//
// Created by kemal on 9/27/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "Lexer.h"
#include "../TestHelpers.h"

// Tests whether combinations of keywords such as "manje ili jednako od" are correctly tokenized as <= symbols and
// concatenated into one token at the lexer level, so that there is no need to handle this in the parser and interpreter.

TEST_CASE("Tokenizes equality", "[lexer]") {
    std::wstring source = L"jednako";

    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token*> tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::DoubleEqual, L"jednako", 1, 0);
}

TEST_CASE("Tokenizes inequality", "[lexer]") {
    std::wstring source = L"nije nije jednako razlicito od";

    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token*> tokens = lexer.tokens;

    REQUIRE(tokens.size() == 4);
    checkToken(tokens[0], TokenType::NotEqual, L"nije", 1, 0);
    checkToken(tokens[1], TokenType::NotEqual, L"nije jednako", 1, 5);
    checkToken(tokens[2], TokenType::NotEqual, L"razlicito od", 1, 18);

}

TEST_CASE("Tokenizes less than", "[lexer]") {
    std::wstring source = L"manje od";

    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token*> tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::Less, L"manje od", 1, 0);
}

TEST_CASE("Tokenizes less than or equal", "[lexer]") {
    std::wstring source = L"manje ili jednako";

    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token*> tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::LessEqual, L"manje ili jednako", 1, 0);
}

TEST_CASE("Tokenizes greater than", "[lexer]") {
    std::wstring source = L"vece od";

    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token*> tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::Greater, L"vece od", 1, 0);
}

TEST_CASE("Tokenizes greater than or equal", "[lexer]") {
    std::wstring source = L"vece ili jednako";

    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token*> tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::GreaterEqual, L"vece ili jednako", 1, 0);
}

TEST_CASE("Tokenizes and", "[lexer]") {
    std::wstring source = L"i";

    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token*> tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::DoubleAmpersand, L"i", 1, 0);
}

TEST_CASE("Tokenizes or", "[lexer]") {
    std::wstring source = L"ili";

    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token*> tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::DoublePipe, L"ili", 1, 0);
}

TEST_CASE("Tokenizes mixed if statement with chained expressions", "[lexer]") {
    std::wstring source = L"ako je a manje ili jednako b ili a vece ili jednako c i c nije null onda"; // misses identifier or something but doesn't matter for lexer

    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token*> tokens = lexer.tokens;

    REQUIRE(tokens.size() == 15);
    checkToken(tokens[0], TokenType::If, L"ako", 1, 0);
    checkToken(tokens[1], TokenType::Is, L"je", 1, 4);
    checkToken(tokens[2], TokenType::Identifier, L"a", 1, 7);
    checkToken(tokens[3], TokenType::LessEqual, L"manje ili jednako", 1, 9);
    checkToken(tokens[4], TokenType::Identifier, L"b", 1, 27);
    checkToken(tokens[5], TokenType::DoublePipe, L"ili", 1, 29);
    checkToken(tokens[6], TokenType::Identifier, L"a", 1, 33);
    checkToken(tokens[7], TokenType::GreaterEqual, L"vece ili jednako", 1, 35);
    checkToken(tokens[8], TokenType::Identifier, L"c", 1, 52);
    checkToken(tokens[9], TokenType::DoubleAmpersand, L"i", 1, 54);
    checkToken(tokens[10], TokenType::Identifier, L"c", 1, 56);
    checkToken(tokens[11], TokenType::NotEqual, L"nije", 1, 58);
    checkToken(tokens[12], TokenType::Null, L"null", 1, 63);
    checkToken(tokens[13], TokenType::Then, L"onda", 1, 68);
}