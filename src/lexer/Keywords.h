//
// Created by kemal on 1/31/2024.
//

#ifndef BAKLAVA_KEYWORDS_H
#define BAKLAVA_KEYWORDS_H

#include <unordered_map>
#include <string>
#include "TokenType.h"

/**
 * @brief Map of all keywords in the language and their corresponding TokenType.
 *
 * Used by the Lexer when an identifier is found to check if it is a keyword.
 * Changing this map enables easy modification of the language's keywords without affecting any other code (except tests).
 */
const std::unordered_map<std::wstring, TokenType> KEYWORDS = {
        {L"ako", TokenType::If},
        {L"inače", TokenType::Else},
        {L"inace", TokenType::Else}, // double
        {L"dok", TokenType::While},
        {L"za", TokenType::For},
        {L"tačno", TokenType::True},
        {L"tacno", TokenType::True}, // double
        {L"netačno", TokenType::False},
        {L"netacno", TokenType::False}, // double
        {L"null", TokenType::Null},
        {L"ispiši", TokenType::Print},
        {L"ispisi", TokenType::Print}, // double
        {L"var", TokenType::Var},
        {L"konst", TokenType::Const},
        {L"funkcija", TokenType::Function},
        {L"vrati", TokenType::Return},
};

#endif //BAKLAVA_KEYWORDS_H
