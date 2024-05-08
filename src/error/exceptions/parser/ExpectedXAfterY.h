//
// Created by kemal on 3/12/2024.
//

#ifndef BAKLAVA_EXPECTEDXAFTERY_H
#define BAKLAVA_EXPECTEDXAFTERY_H

#include "ParserError.h"
#include "Token.h"
#include "ErrorCode.h"

/**
 * @brief Represents an error where a certain token was expected after another token.
 */
class ExpectedXAfterY : public ParserError {
public:
    ExpectedXAfterY(std::wstring expectedWhat, TokenPtr found, TokenPtr after) : ParserError(
            ERROR_EXPECTED_X_AFTER_Y), expectedWhat(expectedWhat), found(std::move(found)),
                                                                                   after(std::move(after)) {
        messageArguments.push_back(expectedWhat);
        messageArguments.push_back(after->value);
    }

    std::wstring expectedWhat;
    TokenPtr found;
    TokenPtr after;
};

#endif //BAKLAVA_EXPECTEDXAFTERY_H
