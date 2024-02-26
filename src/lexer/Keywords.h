//
// Created by kemal on 1/31/2024.
//

#ifndef MATURSKI_2_KEYWORDS_H
#define MATURSKI_2_KEYWORDS_H

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
        {L"istina", TokenType::True},
        {L"neistina", TokenType::False},
        {L"nula", TokenType::Null},
};

#endif //MATURSKI_2_KEYWORDS_H
