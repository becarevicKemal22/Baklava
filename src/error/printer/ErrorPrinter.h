//
// Created by kemal on 1/28/2024.
//

#ifndef MATURSKI_2_ERRORPRINTER_H
#define MATURSKI_2_ERRORPRINTER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <variant>

#include "ErrorCode.h"

typedef std::variant<std::wstring, std::string, const char*> ErrorMessageArgument;
typedef std::pair<std::pair<int, int>, std::wstring> colorHighlight;

/**
 * @brief Base class used for printing error messages to the console.
 *
 * It is passed as a pointer to the lexer, parser and interpreter and is used to print all error messages.
 * All error messages should go through the error printer or a class derived from it.
 */

class ErrorPrinter {
public:
    ErrorPrinter(const std::wstring& source) : source(source) {
        makeLines();
    };
    ~ErrorPrinter() = default;

    /**
     * @brief Prints a simple error message to the console, without any source code or highlighting.
     *
     * Mainly used for errors that occur before the lexer is initialized, most likely 1xx errors.
     * @param errorCode one of the error codes defined in ErrorCode.h
     * @param args vector of arguments that will be used to format the error message, according to the message definition in ErrorMessages.h
     */
    static void printErrorMessage(ErrorCode errorCode, const std::vector<ErrorMessageArgument>& args);

    /**
     * @brief Prints a lexer error message to the console.
     *
     * Prints source code line along with a red caret pointing to the exact position where the error occurred.
     * @param errorCode one of the error codes defined in ErrorCode.h
     * @param line line number where the error occurred
     * @param offset offset on the line where the error occurred
     * @param currentChar index in source of character that caused the error
     */
    void printLexerError(ErrorCode errorCode, unsigned int lineNumber, unsigned int offset, unsigned int currentChar);

private:
    std::wstring source;
    std::vector<std::wstring> lines;
    const int DEFAULT_MARGIN_LEFT = 6;

    void makeLines();

    void printSourceLine(unsigned int line, std::vector<colorHighlight> colorHighlights);
    static std::wstring formattedErrorMessage(ErrorCode errorCode, const std::vector<ErrorMessageArgument>& args);
    void printLineDivider(unsigned int lineNum);
    void printCaretSupportLine(unsigned int offset);
};


#endif //MATURSKI_2_ERRORPRINTER_H
