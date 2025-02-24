//
// Created by kemal on 23-Feb-25.
//

#ifndef BAKLAVA_UNEXPECTEDCHARACTER_H
#define BAKLAVA_UNEXPECTEDCHARACTER_H

#include "LexerError.h"
#include "ErrorCode.h"

/**
 * @brief Represents an error where an invalid character was found in the source code. Lexer errors use 1-based indexing for line and column numbers.
 */
class UnexpectedCharacter : public LexerError {
public:
    UnexpectedCharacter(unsigned int line, unsigned int column, wchar_t character) : LexerError(ERROR_UNEXPECTED_CHARACTER, {}) {
        this->line = line;
        this->column = column;
        messageArguments.push_back(std::wstring(1, character));
        messageArguments.push_back(std::to_string(line));
        messageArguments.push_back(std::to_string(column + 1));
    }
};

#endif //BAKLAVA_UNEXPECTEDCHARACTER_H
