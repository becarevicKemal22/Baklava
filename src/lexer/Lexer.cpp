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
        } else if(c == '{'){
            addToken(TokenType::OpenBrace, c);
            advance();
        } else if(c == '}'){
            addToken(TokenType::ClosedBrace, c);
            advance();
        } else if(c == '['){
            addToken(TokenType::OpenBracket, c);
            advance();
        } else if(c == ']') {
            addToken(TokenType::ClosedBracket, c);
            advance();
        } else if(c == ';'){
            addToken(TokenType::Semicolon, c);
            advance();
        } else if(c == ':'){
            addToken(TokenType::Colon, c);
            advance();
        } else if(c == ','){
            addToken(TokenType::Comma, c);
            advance();
        } else if(c == '.'){
            addToken(TokenType::Dot, c);
            advance();
        } else if(c == '+'){
            if(peek() == '+'){
                addToken(TokenType::DoublePlus, L"++", true);
                advance();
            }else{
                addToken(TokenType::Plus, c);
            }
            advance();
        } else if(c == '-'){
            if(peek() == '-'){
                addToken(TokenType::DoubleMinus, L"--", true);
                advance();
            }else{
                addToken(TokenType::Minus, c);
            }
            advance();
        } else if(c == '*'){
            addToken(TokenType::Star, c);
            advance();
        } else if(c == '/'){
            if(peek() == '/'){
                handleComment();
            } else {
                addToken(TokenType::Slash, c);
                advance();
            }
        } else if(c == '%'){
            addToken(TokenType::Percent, c);
            advance();
        } else if(c == '!'){
            if(peek() == '='){
                addToken(TokenType::BangEqual, L"!=", true);
                advance();
            } else{
                addToken(TokenType::Bang, c);
            }
            advance();
        } else if(c == '<'){
            if(peek() == '='){
                addToken(TokenType::LessEqual, L"<=", true);
                advance();
            } else{
                addToken(TokenType::Less, c);
            }
            advance();
        } else if(c == '>'){
            if(peek() == '='){
                addToken(TokenType::GreaterEqual, L">=", true);
                advance();
            } else{
                addToken(TokenType::Greater, c);
            }
            advance();
        } else if(c == '='){
            if(peek() == '='){
                addToken(TokenType::DoubleEqual, L"==", true);
                advance();
            } else{
                addToken(TokenType::Equal, c);
            }
            advance();
        } else if(c == '|'){
            if(peek() == '|'){
                addToken(TokenType::DoublePipe, L"||", true);
                advance();
                advance();
            } else{
                if(printer != nullptr){
                    printer->printLexerError(ERROR_UNEXPECTED_CHARACTER, line, charIndexOnLine, currentChar);
                }
                throw std::runtime_error("Single Pipe");
            }
        } else if(c == '&'){
            if(peek() == '&'){
                addToken(TokenType::DoubleAmpersand, L"&&", true);
                advance();
                advance();
            } else{
                if(printer != nullptr){
                    printer->printLexerError(ERROR_UNEXPECTED_CHARACTER, line, charIndexOnLine, currentChar);
                }
                throw std::runtime_error("Single ampersand");
            }
        }

        else if(c == '"' || c == '\''){
            handleString(c);
        }

        else if(c == ' '){
            advance();
        } else if(c == '\t'){
            handleTab();
        } else if(c == '\n' || c == '\r'){
            handleNewLine();
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
        } else{
            if(printer != nullptr){
                printer->printLexerError(ERROR_UNEXPECTED_CHARACTER, line, charIndexOnLine, currentChar);
            }
            throw std::runtime_error("");
        }
        // Add custom exception to be caught in main.cpp

    }
    addToken(TokenType::Eof, L"");
}

void Lexer::addToken(TokenType type, wchar_t character) {
    tokens.push_back(new Token(type, std::wstring(1, character), line, charIndexOnLine));
}

void Lexer::addToken(TokenType type, const std::wstring& value, bool offsetIsStartOfToken) {
    if(offsetIsStartOfToken)
        tokens.push_back(new Token(type, value, line, charIndexOnLine));
    else
        tokens.push_back(new Token(type, value, line, (charIndexOnLine + 1) - value.length()));
}

void Lexer::addStringToken(const std::wstring& value, unsigned int line, unsigned int offset){
    tokens.push_back(new Token(TokenType::String, value, line, offset));
}

void Lexer::handleTab(){
    advance();
    charIndexOnLine += 3;
}

void Lexer::handleNewLine() {
    line++;
    if(source[currentChar] == '\r' && source[currentChar + 1] == '\n'){
        currentChar++;
    }
    currentChar++;
    charIndexOnLine = 0;
}

void Lexer::handleString(wchar_t startQuote){
    unsigned int startLine = line;
    unsigned int startOffset = charIndexOnLine;
    unsigned int startChar = currentChar;
    advance();
    std::wstring string;
    while(source[currentChar] != '\0'){
        if(source[currentChar] == startQuote){
            advance();
            addStringToken(string, startLine, startOffset);
            return;
        }
        string += source[currentChar];
        advance();
    }
    if(printer != nullptr){
        printer->printLexerError(ERROR_UNTERMINATED_STRING, startLine, startOffset, startChar);
    }
    throw std::runtime_error("Unterminated string");
}

void Lexer::handleComment() {
    while(source[currentChar] != '\n' && source[currentChar] != '\0'){
        advance();
    }
    handleNewLine();
}