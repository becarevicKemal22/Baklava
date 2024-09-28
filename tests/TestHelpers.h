//
// Created by kemal on 2/5/2024.
//

#ifndef BAKLAVA_TESTHELPERS_H
#define BAKLAVA_TESTHELPERS_H

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

void checkToken(const Token* token, TokenType type, const std::wstring& value);
void checkToken(const Token* token, TokenType type, const std::wstring& value, unsigned int line, unsigned int charIndexOnLine);

#endif //BAKLAVA_TESTHELPERS_H
