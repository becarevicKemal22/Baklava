//
// Created by kemal on 3/14/2024.
//

#include <catch2/catch_test_macros.hpp>
#include "Environment.h"
#include "UndeclaredVariable.h"
#include "VariableRedeclaration.h"
#include "ConstReassignment.h"
#include "Object.h"

Token *createMockToken(TokenType type, std::wstring value) {
    return new Token(type, value, 0, 0);
}

TEST_CASE("Defines and gets variable", "[environment]") {
    Environment env;
    Token *token = createMockToken(TokenType::Identifier, L"a");
    env.define(token, {ValueType::Number, {.number = 5}}, false);
    REQUIRE(env.get(token).as.number == 5);
}

TEST_CASE("Assigns variable", "[environment]") {
    Environment env;
    Token *token = createMockToken(TokenType::Identifier, L"a");
    env.define(token, {ValueType::Number, {.number = 5}}, false);
    env.assign(token, {ValueType::Number, {.number = 10}});
    REQUIRE(env.get(token).as.number == 10);
}

TEST_CASE("Assigns variable to another variable", "[environment]") {
    Environment env;
    Token *tokenA = createMockToken(TokenType::Identifier, L"a");
    Token *tokenB = createMockToken(TokenType::Identifier, L"b");
    env.define(tokenA, {ValueType::Number, {.number = 5}}, false);
    env.define(tokenB, {ValueType::Number, {.number = 10}}, false);
    env.assign(tokenA, env.get(tokenB));
    REQUIRE(env.get(tokenA).as.number == 10);
    env.assign(tokenB, {ValueType::Number, {.number = 15}});
    REQUIRE(env.get(tokenA).as.number == 10);
    REQUIRE(env.get(tokenB).as.number == 15);
}

TEST_CASE("Assigns string variable to another variable correctly", "[environment]") {
    Environment env;
    Token *tokenA = createMockToken(TokenType::Identifier, L"a");
    Token *tokenB = createMockToken(TokenType::Identifier, L"b");
    auto aString = new ObjectString();
    aString->value = L"value";
    env.define(tokenA, {ValueType::Null}, false);
    env.define(tokenB, {ValueType::Object, {.object = (Object*)aString}}, false);
    env.assign(tokenA, env.get(tokenB));
    RuntimeValue varA = env.get(tokenA);
    REQUIRE(IS_OBJ(varA));
    REQUIRE(IS_STRING_OBJ(varA));
    REQUIRE(AS_STRING_OBJ(varA)->value == L"value");

    auto bString = new ObjectString();
    bString->value = L"newValue";
    RuntimeValue newValue = {ValueType::Object, {.object = (Object*)bString}};
    env.assign(tokenB, newValue);
    varA = env.get(tokenA);
    REQUIRE(IS_OBJ(varA));
    REQUIRE(IS_STRING_OBJ(varA));
    REQUIRE(AS_STRING_OBJ(varA)->value == L"value");
    RuntimeValue varB = env.get(tokenB);
    REQUIRE(IS_OBJ(varB));
    REQUIRE(IS_STRING_OBJ(varB));
    REQUIRE(AS_STRING_OBJ(varB)->value == L"newValue");
}

TEST_CASE("Throws error when getting undefined variable", "[environment]") {
    Environment env;
    Token *token = createMockToken(TokenType::Identifier, L"a");
    REQUIRE_THROWS_AS(env.get(token), UndeclaredVariable);
}

TEST_CASE("Throws error when defining variable twice", "[environment]") {
    Environment env;
    Token *token = createMockToken(TokenType::Identifier, L"a");
    env.define(token, {ValueType::Number, {.number = 5}}, false);
    REQUIRE_THROWS_AS(env.define(token, {ValueType::Number, {.number = 10}}, false), VariableRedeclaration);
}

TEST_CASE("Throws error on const reassignment", "[environment]") {
    Environment env;
    Token *token = createMockToken(TokenType::Identifier, L"a");
    env.define(token, {ValueType::Number, {.number = 5}}, true);
    REQUIRE_THROWS_AS(env.assign(token, {ValueType::Number, {.number = 10}}), ConstReassignment);
}