//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_TESTHELPERS_H
#define MATURSKI_2_TESTHELPERS_H

#include <catch2/catch_test_macros.hpp>

#include "Statement.h"
#include "ExpressionStatement.h"
#include "Token.h"
#include "Interpreter.h"

template<typename T>
T* getNode(Statement* node) {
    if(dynamic_cast<ExpressionStatement*>(node) != nullptr) {
        return getNode<T>(dynamic_cast<ExpressionStatement*>(node)->expr);
    }
    T* castedNode = dynamic_cast<T*>(node);
    REQUIRE(castedNode != nullptr);
    return castedNode;
}

template<typename T>
void checkHandledError(Interpreter* interpreter){
    REQUIRE(interpreter->handledError != nullptr);
    auto error = dynamic_cast<T*>(interpreter->handledError);
    REQUIRE(error != nullptr);
}

std::unique_ptr<Program> parseSource(const std::wstring& source);
Expression* parseSingleExpression(const std::wstring& source);
Token* makeToken(TokenType type, const std::wstring& lexeme);

#endif //MATURSKI_2_TESTHELPERS_H
