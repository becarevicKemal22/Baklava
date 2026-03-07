//
// Created by kemal on 3/14/2024.
//

#include <catch2/catch_test_macros.hpp>
#include "Environment.h"
#include "UndeclaredIdentifier.h"
#include "IdentifierRedeclaration.h"
#include "ConstReassignment.h"
#include "Object.h"

Token *createMockToken(TokenType type, std::wstring value) {
    return new Token(type, value, 0, 0);
}

TEST_CASE("Defines and gets variable", "[environment]") {
    Environment *env = new Environment(nullptr);
    env->addRef();
    Token *token = createMockToken(TokenType::Identifier, L"a");
    env->define(token, {ValueType::Number, {.number = 5}}, false);
    REQUIRE(env->get(token).as.number == 5);
    env->release();
    delete token;
}

TEST_CASE("Assigns variable", "[environment]") {
    Environment *env = new Environment(nullptr);
    env->addRef();
    Token *token = createMockToken(TokenType::Identifier, L"a");
    env->define(token, {ValueType::Number, {.number = 5}}, false);
    env->assign(token, {ValueType::Number, {.number = 10}});
    REQUIRE(env->get(token).as.number == 10);
    env->release();
    delete token;
}

TEST_CASE("Assigns variable to another variable", "[environment]") {
    Environment *env = new Environment(nullptr);
    env->addRef();
    Token *tokenA = createMockToken(TokenType::Identifier, L"a");
    Token *tokenB = createMockToken(TokenType::Identifier, L"b");
    env->define(tokenA, {ValueType::Number, {.number = 5}}, false);
    env->define(tokenB, {ValueType::Number, {.number = 10}}, false);
    env->assign(tokenA, env->get(tokenB));
    REQUIRE(env->get(tokenA).as.number == 10);
    env->assign(tokenB, {ValueType::Number, {.number = 15}});
    REQUIRE(env->get(tokenA).as.number == 10);
    REQUIRE(env->get(tokenB).as.number == 15);
    env->release();
    delete tokenA;
    delete tokenB;
}

TEST_CASE("Assigns string variable to another variable correctly", "[environment]") {
    Environment *env = new Environment(nullptr);
    env->addRef();
    Token *tokenA = createMockToken(TokenType::Identifier, L"a");
    Token *tokenB = createMockToken(TokenType::Identifier, L"b");
    auto aString = new ObjectString();
    aString->value = L"value";
    env->define(tokenA, {ValueType::Null}, false);
    env->define(tokenB, {ValueType::Object, {.object = (Object *) aString}}, false);
    env->assign(tokenA, env->get(tokenB));
    RuntimeValue varA = env->get(tokenA);
    REQUIRE(IS_OBJ(varA));
    REQUIRE(IS_STRING_OBJ(varA));
    REQUIRE(AS_STRING_OBJ(varA)->value == L"value");

    auto bString = new ObjectString();
    bString->value = L"newValue";
    RuntimeValue newValue = {ValueType::Object, {.object = (Object*)bString}};
    env->assign(tokenB, newValue);
    varA = env->get(tokenA);
    REQUIRE(IS_OBJ(varA));
    REQUIRE(IS_STRING_OBJ(varA));
    REQUIRE(AS_STRING_OBJ(varA)->value == L"value");
    RuntimeValue varB = env->get(tokenB);
    REQUIRE(IS_OBJ(varB));
    REQUIRE(IS_STRING_OBJ(varB));
    REQUIRE(AS_STRING_OBJ(varB)->value == L"newValue");
    env->release();
    delete tokenA;
    delete tokenB;
}

TEST_CASE("Throws error when getting undefined variable", "[environment]") {
    Environment *env = new Environment(nullptr);
    env->addRef();
    Token *token = createMockToken(TokenType::Identifier, L"a");
    REQUIRE_THROWS_AS(env->get(token), UndeclaredIdentifier);
    env->release();
    delete token;
}

TEST_CASE("Throws error when defining variable twice", "[environment]") {
    Environment *env = new Environment(nullptr);
    env->addRef();
    Token *token = createMockToken(TokenType::Identifier, L"a");
    env->define(token, {ValueType::Number, {.number = 5}}, false);
    REQUIRE_THROWS_AS(env->define(token, {ValueType::Number, {.number = 10}}, false), IdentifierRedeclaration);
    env->release();
    delete token;
}

TEST_CASE("Throws error on const reassignment", "[environment]") {
    Environment *env = new Environment(nullptr);
    env->addRef();
    Token *token = createMockToken(TokenType::Identifier, L"a");
    env->define(token, {ValueType::Number, {.number = 5}}, true);
    REQUIRE_THROWS_AS(env->assign(token, {ValueType::Number, {.number = 10}}), ConstReassignment);
    env->release();
    delete token;
}

//****************************************
// Parents environments and shadowing
//****************************************

TEST_CASE("Gets variable from parent scope", "[environment]"){
    Environment *global = new Environment(nullptr);
    global->addRef();
    Environment *local = new Environment(global);
    local->addRef();
    Token *token = createMockToken(TokenType::Identifier, L"a");
    global->define(token, {ValueType::Number, {.number = 5}}, false);
    REQUIRE(local->get(token).as.number == 5);
    local->release();
    global->release();
    delete token;
}

TEST_CASE("Assigns variable from parent scope", "[environment]") {
    Environment *global = new Environment(nullptr);
    global->addRef();
    Environment *local = new Environment(global);
    local->addRef();
    Token *token = createMockToken(TokenType::Identifier, L"a");
    global->define(token, {ValueType::Number, {.number = 5}}, false);
    local->assign(token, {ValueType::Number, {.number = 10}});
    REQUIRE(global->get(token).as.number == 10);
    REQUIRE(local->get(token).as.number == 10);
    local->release();
    global->release();
    delete token;
}

TEST_CASE("Shadowing variable", "[environment]") {
    Environment *global = new Environment(nullptr);
    global->addRef();
    Environment *local = new Environment(global);
    local->addRef();
    Token *token = createMockToken(TokenType::Identifier, L"a");
    global->define(token, {ValueType::Number, {.number = 5}}, false);
    local->define(token, {ValueType::Number, {.number = 10}}, false);
    REQUIRE(local->get(token).as.number == 10);
    REQUIRE(global->get(token).as.number == 5);
    global->assign(token, {ValueType::Number, {.number = 15}});
    REQUIRE(local->get(token).as.number == 10);
    REQUIRE(global->get(token).as.number == 15);
    local->assign(token, {ValueType::Number, {.number = 20}});
    REQUIRE(local->get(token).as.number == 20);
    REQUIRE(global->get(token).as.number == 15);
    local->release();
    global->release();
    delete token;
}

TEST_CASE("Throws on parent const reassignment", "[environment]") {
    Environment *global = new Environment(nullptr);
    global->addRef();
    Environment *local = new Environment(global);
    local->addRef();
    Token *token = createMockToken(TokenType::Identifier, L"a");
    global->define(token, {ValueType::Number, {.number = 5}}, true);
    REQUIRE_THROWS_AS(local->assign(token, {ValueType::Number, {.number = 10}}), ConstReassignment);
    local->release();
    global->release();
    delete token;
}

TEST_CASE("Throws on undefined variable in parent scopes", "[environment]") {
    Environment *global = new Environment(nullptr);
    global->addRef();
    Environment *local = new Environment(global);
    local->addRef();
    Token *token = createMockToken(TokenType::Identifier, L"a");
    REQUIRE_THROWS_AS(local->get(token), UndeclaredIdentifier);
    local->release();
    global->release();
    delete token;
}
