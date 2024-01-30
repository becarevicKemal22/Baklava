//
// Created by kemal on 1/28/2024.
//


#include <stdexcept>

#include "Lexer.h"

void Lexer::tokenize() {
    unsigned int length = source.length();
    while (currentChar < length) {
        wchar_t c = source[currentChar];
        switch (c) {
            case '(':
                addToken(TokenType::OpenParenthesis, line, charIndexOnLine);
                break;
            case ')':
                addToken(TokenType::ClosedParenthesis, line, charIndexOnLine);
                break;
            default:
                if(printer != nullptr){
                    printer->printLexerError(ERROR_UNEXPECTED_CHARACTER, line, charIndexOnLine, currentChar);
                }
                throw std::runtime_error("");
                // Add custom exception to be caught in main.cpp
        }

    }
}

void Lexer::addToken(TokenType type, unsigned int line, unsigned int offset) {
    tokens.emplace_back(type, line, offset);
}