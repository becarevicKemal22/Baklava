//
// Created by kemal on 23-Feb-25.
//

#ifndef BAKLAVA_UNTERMINATEDSTRING_H
#define BAKLAVA_UNTERMINATEDSTRING_H

#include "LexerError.h"
#include "ErrorCode.h"

/**
 * @brief Represents an error where an unterminated string was found. Lexer errors use 1-based indexing for line and column numbers.
 */
class UnterminatedString : public LexerError {
public:
    UnterminatedString(unsigned int line, unsigned int column) : LexerError(ERROR_UNTERMINATED_STRING, {}) {
        this->line = line;
        this->column = column;
        messageArguments.push_back(std::to_string(line));
        messageArguments.push_back(std::to_string(column + 1));
    }
};

#endif //BAKLAVA_UNTERMINATEDSTRING_H
