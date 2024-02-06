//
// Created by kemal on 1/31/2024.
//

#ifndef MATURSKI_2_KEYWORDS_H
#define MATURSKI_2_KEYWORDS_H

#include <unordered_map>
#include <string>
#include "TokenType.h"

const std::unordered_map<std::wstring, TokenType> KEYWORDS = {
        {L"ako", TokenType::If},
        {L"istina", TokenType::True},
        {L"neistina", TokenType::False},
        {L"nula", TokenType::Null},
};

#endif //MATURSKI_2_KEYWORDS_H
