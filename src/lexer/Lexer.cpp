//
// Created by kemal on 1/28/2024.
//


#include <stdexcept>
#include <iostream>

#include "Lexer.h"
#include "Keywords.h"

void Lexer::tokenize() {
    std::setlocale(LC_ALL, "bs_BA.UTF-8");
    unsigned int length = source.length();
    while (currentChar < length) {
        wchar_t c = source[currentChar];
        if(c == '('){
            addToken(TokenType::OpenParenthesis, c);
            advance();
        } else if(c == ')'){
            addToken(TokenType::ClosedParenthesis, c);
            advance();
        }



        else if(iswalnum(c)){
            if(iswalpha(c)){
                std::wstring identifier;
                identifier += c;
                while((iswalnum(peek()) || peek() == '_') && peek() != '\0'){
                    advance();
                    identifier += source[currentChar];
                }
                TokenType type = TokenType::Identifier;
                auto it = KEYWORDS.find(identifier);
                if(it != KEYWORDS.end()){
                    type = it->second;
                }
                addToken(type, identifier);
                advance();
            }else{
                std::wstring number;
                number += c;
                while((iswdigit(peek()) || (peek() == '.' && iswdigit(source[currentChar + 2]))) && peek() != '\0'){
                    advance();
                    number += source[currentChar];
                }
                addToken(TokenType::Number, number);
                advance();
            }
        }

//                if(printer != nullptr){
//                    printer->printLexerError(ERROR_UNEXPECTED_CHARACTER, line, charIndexOnLine, currentChar);
//                }
//                throw std::runtime_error("");
                // Add custom exception to be caught in main.cpp

    }
}

void Lexer::addToken(TokenType type, wchar_t character) {
    tokens.emplace_back(type, std::to_wstring(character), line, charIndexOnLine);
}

void Lexer::addToken(TokenType type, const std::wstring& value) {
    tokens.emplace_back(type, value, line, (charIndexOnLine + 1) - value.length());
}