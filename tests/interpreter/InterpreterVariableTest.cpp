//
// Created by kemal on 3/15/2024.
//

#include "catch2/catch_test_macros.hpp"

#include "Interpreter.h"
#include "Program.h"
#include "RuntimeValue.h"
#include "Object.h"
#include "../TestHelpers.h"

TEST_CASE("Declares variable and assigns value", "[interpreter][variable]") {
    std::wstring source = L"var x = 5;";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.environments.top().get(makeToken(TokenType::Identifier, L"x")).as.number == 5);
}

TEST_CASE("Declares variable and assigns string value", "[interpreter][variable]") {
    std::wstring source = L"var x = \"hello\";";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.environments.top().get(makeToken(TokenType::Identifier, L"x"))) == L"hello");
}

TEST_CASE("Declares variable and reassigns value", "[interpreter][variable]") {
    std::wstring source = L"var x = 5; x = 10;";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.environments.top().get(makeToken(TokenType::Identifier, L"x")).as.number == 10);
}

TEST_CASE("Copies value on variable-to-variable assignment", "[interpreter][variable]") {
    std::wstring source = L"var x = 5; var y = x; x = 10;";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(interpreter.environments.top().get(makeToken(TokenType::Identifier, L"y")).as.number == 5);
    REQUIRE(interpreter.environments.top().get(makeToken(TokenType::Identifier, L"x")).as.number == 10);
}

TEST_CASE("Copies value on variable-to-variable string assignment", "[interpreter][variable]") {
    std::wstring source = L"var x = \"hello\"; var y = x; x = \"world\";";
    Interpreter interpreter;

    interpreter.interpret(parseSource(source).get());
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.environments.top().get(makeToken(TokenType::Identifier, L"y"))) == L"hello");
    REQUIRE(GET_STRING_OBJ_VALUE(interpreter.environments.top().get(makeToken(TokenType::Identifier, L"x"))) == L"world");
}
