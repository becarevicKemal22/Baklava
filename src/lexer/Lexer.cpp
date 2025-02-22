//
// Created by kemal on 1/28/2024.
//


#include <stdexcept>
#include <iostream>

#include "Lexer.h"
#include "Keywords.h"
#include "KeywordCombinations.h"

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
                addToken(TokenType::NotEqual, L"!=", true);
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

                if(type == TokenType::Identifier){
                    addToken(type, identifier, false);
                }else{
                    keywordHistory.push_back(generateToken(type, identifier, false));
                }

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
    handleKeywordCombinations();
}

void Lexer::addToken(TokenType type, const std::wstring& value, bool offsetIsStartOfToken) {
    tokens.push_back(generateToken(type, value, offsetIsStartOfToken));
    handleKeywordCombinations();
}

TokenPtr Lexer::generateToken(TokenType type, const std::wstring& value, bool offsetIsStartOfToken) {
    if(offsetIsStartOfToken)
        return new Token(type, value, line, charIndexOnLine);
    else
        return new Token(type, value, line, (charIndexOnLine + 1) - value.length());
}

void Lexer::addStringToken(const std::wstring& value, unsigned int line, unsigned int offset){
    tokens.push_back(new Token(TokenType::String, value, line, offset));
    handleKeywordCombinations();
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

void Lexer::handleKeywordCombinations() {
    if(keywordHistory.empty() || tokens.empty()) return;

    Token* lastToken = tokens.back();
    if(isKeyword(lastToken)) return;
    if(lastToken->offset < keywordHistory.front()->offset) return; // if this is true, it means that there hasn't actually been any new non-keyword tokens since the last time keyword combinations were handled.

    tokens.pop_back(); // removes the token from the end, but it will get returned later

    std::vector<TokenType> types;
    for(auto token: keywordHistory){
        types.push_back(token->type);
    }
    while(!keywordHistory.empty()){
        size_t n = types.size();
        while(!isCombination(std::vector<TokenType>(types.begin(), types.begin() + n))){
            if(n == 1){
                break;
            }

            n--;
        }

        if(isCombination(std::vector<TokenType>(types.begin(), types.begin() + n))){
            TokenType type;
            Combination combination = std::vector<TokenType>(types.begin(), types.begin() + n);
            for(auto _combination: KEYWORD_COMBINATIONS){
                if(_combination.first == combination){
                    type = _combination.second;
                    break;
                }
            }

            tokens.push_back(combineConsecutiveTokens(std::vector<TokenPtr>(keywordHistory.begin(), keywordHistory.begin() + n), type));
            keywordHistory.erase(keywordHistory.begin(), keywordHistory.begin() + n);
            types.erase(types.begin(), types.begin() + n);
        }else{
            tokens.push_back(keywordHistory.front());
            keywordHistory.erase(keywordHistory.begin());
            types.erase(types.begin());
        }
    }

    tokens.push_back(lastToken);
}

bool Lexer::isKeyword(Token* token) {
    return KEYWORDS.find(token->value) != KEYWORDS.end();
}

bool Lexer::isCombination(const std::vector<TokenType>& types) {
    for(auto combination: KEYWORD_COMBINATIONS){
        if(combination.first == types){
            return true;
        }
    }
    return false;
}

TokenPtr Lexer::combineConsecutiveTokens(const std::vector<TokenPtr>& tokens, TokenType newType) {
    std::wstring value;
    size_t n = tokens.size();
    for(size_t i = 0; i < n; i++){
        value += tokens[i]->value;
        if(i != n - 1){
            value += std::wstring(tokens[i + 1]->offset - (tokens[i]->offset + tokens[i]->value.length()), L' ');
        }

    }
    return new Token(newType, value, tokens[0]->line, tokens[0]->offset);
}