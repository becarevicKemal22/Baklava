//
// Created by kemal on 3/12/2024.
//

#ifndef MATURSKI_2_EXPECTEDXBEFOREY_H
#define MATURSKI_2_EXPECTEDXBEFOREY_H

#include "ParserError.h"

/**
 * @brief Represents an error where a certain token was expected before another token.
 */
class ExpectedXBeforeY : public ParserError {
public:
    ExpectedXBeforeY(std::wstring expectedWhat, TokenPtr found, TokenPtr before) : ParserError(
            ERROR_EXPECTED_X_BEFORE_Y), expectedWhat(expectedWhat), found(std::move(found)),
                                                                                   before(std::move(before)) {
        messageArguments.push_back(expectedWhat);
        if (before->type == TokenType::Eof) before->value = L"kraj datoteke";
        messageArguments.push_back(before->value);
    }

    std::wstring expectedWhat;
    TokenPtr found;
    TokenPtr before;
};

#endif //MATURSKI_2_EXPECTEDXBEFOREY_H
