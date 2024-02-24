#include <catch2/catch_test_macros.hpp>

#include "Lexer.h"

void checkToken(const Token* token, TokenType type, const std::wstring& value){
    CHECK(token->type == type);
    CHECK(token->value == value);
}

void checkToken(const Token* token, TokenType type, const std::wstring& value, unsigned int line, unsigned int charIndexOnLine){
    checkToken(token, type, value);
    CHECK(token->line == line);
    CHECK(token->offset == charIndexOnLine);
}

TEST_CASE("Adds eof to empty input", "[lexer]"){
    std::wstring source = L"";
    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token*> tokens = lexer.tokens;
    REQUIRE(tokens.size() == 1);
    checkToken(tokens[0], TokenType::Eof, L"");
}

TEST_CASE("Tokenizes identifiers and numbers", "[lexer]") {
    std::wstring source = L"24.2 ident_2 24 ćčđšž";

    Lexer lexer(source);
    lexer.tokenize();
    std::vector<Token*> tokens = lexer.tokens;

    REQUIRE(tokens.size() == 5);
    checkToken(tokens[0], TokenType::Number, L"24.2", 1, 0);
    checkToken(tokens[1], TokenType::Identifier, L"ident_2", 1, 5);
    checkToken(tokens[2], TokenType::Number, L"24", 1, 13);
    checkToken(tokens[3], TokenType::Identifier, L"ćčđšž", 1, 16);
}

TEST_CASE("Correctly handles tabs", "[lexer]"){
    std::wstring source = L"\tident";

    Lexer lexer(source);
    lexer.tokenize();
    auto tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::Identifier, L"ident", 1, 4);
}

TEST_CASE("Correctly handles new lines", "[lexer]"){
    std::wstring source = L"\n\r ident";

    Lexer lexer(source);
    lexer.tokenize();
    auto tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::Identifier, L"ident", 3, 1);
}

TEST_CASE("Correctly handles spaces", "[lexer]"){
    std::wstring source = L" 1  2";

    Lexer lexer(source);
    lexer.tokenize();
    auto tokens = lexer.tokens;

    REQUIRE(tokens.size() == 3);
    checkToken(tokens[0], TokenType::Number, L"1", 1, 1);
    checkToken(tokens[1], TokenType::Number, L"2", 1, 4);
}

TEST_CASE("Correctly handles multiple escape characters", "[lexer]"){
    std::wstring source = L"\n\t1 2\r  3";

    Lexer lexer(source);
    lexer.tokenize();
    auto tokens = lexer.tokens;

    REQUIRE(tokens.size() == 4);
    checkToken(tokens[0], TokenType::Number, L"1", 2, 4);
    checkToken(tokens[1], TokenType::Number, L"2", 2, 6);
    checkToken(tokens[2], TokenType::Number, L"3", 3, 2);
}

TEST_CASE("Tokenizes empty string", "[lexer]"){
    std::wstring source = L"\"\"";

    Lexer lexer(source);
    lexer.tokenize();
    auto tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::String, L"", 1, 0);
}

TEST_CASE("Tokenizes string", "[lexer]"){
    std::wstring source = L"\"neki stringić\"";

    Lexer lexer(source);
    lexer.tokenize();
    auto tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::String, L"neki stringić", 1, 0);
}

TEST_CASE("Throws error on unterminated string", "[lexer]"){
    std::wstring source = L"\"";

    Lexer lexer(source);
    REQUIRE_THROWS(lexer.tokenize());
}

TEST_CASE("Tokenizes special characters", "[lexer]"){
    std::wstring source = L"(){}[];:,.";

    Lexer lexer(source);
    lexer.tokenize();
    auto tokens = lexer.tokens;

    REQUIRE(tokens.size() == 11);
    checkToken(tokens[0], TokenType::OpenParenthesis, L"(");
    checkToken(tokens[1], TokenType::ClosedParenthesis, L")");
    checkToken(tokens[2], TokenType::OpenBrace, L"{");
    checkToken(tokens[3], TokenType::ClosedBrace, L"}");
    checkToken(tokens[4], TokenType::OpenBracket, L"[");
    checkToken(tokens[5], TokenType::ClosedBracket, L"]");
    checkToken(tokens[6], TokenType::Semicolon, L";");
    checkToken(tokens[7], TokenType::Colon, L":");
    checkToken(tokens[8], TokenType::Comma, L",");
    checkToken(tokens[9], TokenType::Dot, L".");
    checkToken(tokens[10], TokenType::Eof, L"");
}

TEST_CASE("Tokenizes operators", "[lexer]"){
    std::wstring source = L"+-*/%! != < > <= >= = == ||&&";

    Lexer lexer(source);
    lexer.tokenize();
    auto tokens = lexer.tokens;

    REQUIRE(tokens.size() == 16);
    checkToken(tokens[0], TokenType::Plus, L"+", 1, 0);
    checkToken(tokens[1], TokenType::Minus, L"-", 1, 1);
    checkToken(tokens[2], TokenType::Star, L"*", 1, 2);
    checkToken(tokens[3], TokenType::Slash, L"/", 1, 3);
    checkToken(tokens[4], TokenType::Percent, L"%", 1, 4);
    checkToken(tokens[5], TokenType::Bang, L"!", 1, 5);
    checkToken(tokens[6], TokenType::BangEqual, L"!=", 1, 7);
    checkToken(tokens[7], TokenType::Less, L"<", 1, 10);
    checkToken(tokens[8], TokenType::Greater, L">", 1, 12);
    checkToken(tokens[9], TokenType::LessEqual, L"<=", 1, 14);
    checkToken(tokens[10], TokenType::GreaterEqual, L">=", 1, 17);
    checkToken(tokens[11], TokenType::Equal, L"=", 1, 20);
    checkToken(tokens[12], TokenType::DoubleEqual, L"==", 1, 22);
    checkToken(tokens[13], TokenType::DoublePipe, L"||", 1, 25);
    checkToken(tokens[14], TokenType::DoubleAmpersand, L"&&", 1, 27);
}

TEST_CASE("Tokenizes keywords", "[lexer]"){
    std::wstring source = L"ako";

    Lexer lexer(source);
    lexer.tokenize();
    auto tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::If, L"ako", 1, 0);
    checkToken(tokens[1], TokenType::Eof, L"");
}

TEST_CASE("Ignores comments", "[lexer]"){
    std::wstring source = L"// ovo je komentar\nako";

    Lexer lexer(source);
    lexer.tokenize();
    auto tokens = lexer.tokens;

    REQUIRE(tokens.size() == 2);
    checkToken(tokens[0], TokenType::If, L"ako", 2, 0);
}