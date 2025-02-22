//
// Created by kemal on 9/28/2024.
//

#ifndef BAKLAVA_KEYWORDCOMBINATIONS_H
#define BAKLAVA_KEYWORDCOMBINATIONS_H

#include <unordered_map>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include "TokenType.h"

using std::array;
using std::vector;
using std::pair;

using Combination = vector<TokenType>;

/**
 * @brief Array of all keyword combinations in the language and what their resulting type should be.
 * For example, "manje ili jednako" should be concatenated into one token of type LessEqual.
 */
const array<pair<Combination, TokenType>, 12> KEYWORD_COMBINATIONS = {
        pair<Combination, TokenType>({TokenType::Manje, TokenType::Ili, TokenType::Jednako}, TokenType::LessEqual),
        pair<Combination, TokenType>({TokenType::Vece, TokenType::Ili, TokenType::Jednako}, TokenType::GreaterEqual),
        pair<Combination, TokenType>({TokenType::Vece, TokenType::Od}, TokenType::Greater),
        pair<Combination, TokenType>({TokenType::Manje, TokenType::Od}, TokenType::Less),
        pair<Combination, TokenType>({TokenType::Nije, TokenType::Jednako}, TokenType::NotEqual),
        pair<Combination, TokenType>({TokenType::Razlicito, TokenType::Od}, TokenType::NotEqual),

        // some of these can be used standalone, so they need to be 'single' combinations as well, since they are not
        // associated the type they are when single in KEYWORDS.

        pair<Combination, TokenType>({TokenType::Jednako}, TokenType::DoubleEqual),
        pair<Combination, TokenType>({TokenType::Nije}, TokenType::NotEqual),
        pair<Combination, TokenType>({TokenType::Manje}, TokenType::Less),
        pair<Combination, TokenType>({TokenType::Vece}, TokenType::Greater),
        pair<Combination, TokenType>({TokenType::I}, TokenType::DoubleAmpersand),
        pair<Combination, TokenType>({TokenType::Ili}, TokenType::DoublePipe),
};

#endif //BAKLAVA_KEYWORDCOMBINATIONS_H
