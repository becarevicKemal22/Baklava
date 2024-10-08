//
// Created by kemal on 1/28/2024.
//

#ifndef BAKLAVA_ERRORPRINTER_H
#define BAKLAVA_ERRORPRINTER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <variant>

#include "ErrorCode.h"
#include "ErrorMessageArgument.h"
#include "RuntimeError.h"
#include "WrongTypeError.h"
#include "WrongBinaryOperandTypes.h"
#include "ParserError.h"
#include "ExpectedXBeforeY.h"
#include "ExpectedXAfterY.h"
#include "UninitializedConst.h"
#include "VariableRedeclaration.h"
#include "UndeclaredVariable.h"
#include "ConstReassignment.h"
#include "InvalidLValue.h"
#include "InvalidCall.h"
#include "TooManyArguments.h"
#include "TooFewArguments.h"
#include "InvalidReturnPosition.h"
#include "SelfReferencingInitializer.h"
#include "IndexOutOfBounds.h"
#include "NonIntegerIndex.h"
#include "IndexingNonArray.h"
#include "InvalidDefaultParameterPosition.h"
#include "InvalidDefaultParameterValue.h"

typedef std::pair<std::pair<int, int>, std::wstring> colorHighlight;

// Forward declaration only, explanation as to why this is not a method of the class is found in the cpp file.
bool isOutputANSICompatible();

/**
 * @brief Base class used for printing error messages to the console.
 *
 * Should be passed as a pointer to the lexer, parser and interpreter and is used to print all error messages.
 * All error messages should go through the error printer or a class derived from it.
 */
class ErrorPrinter {
public:
    ErrorPrinter(const std::wstring& source) : source(source) {
        makeLines();
        if(!isOutputANSICompatible()){
            ANSI_RED = L"";
            ANSI_RESET = L"";
            ANSI_GREEN = L"";
            ANSI_BLUE = L"";
        }
    };
    ~ErrorPrinter() = default;

    /**
     * @brief Prints a simple error message to the console, without any source code or highlighting.
     *
     * Mainly used for errors that occur before the lexer is initialized, most likely 1xx errors.
     * @param errorCode one of the error codes defined in ErrorCode.h
     * @param args vector of arguments that will be used to format the error message, according to the message definition in ErrorMessages.h
     */
    void printErrorMessage(ErrorCode errorCode, const std::vector<ErrorMessageArgument>& args);

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

    void printParserError(const ParserError* error);

    void printRuntimeError(const RuntimeError* error);
private:

    std::wstring ANSI_RED = L"\033[31m";
    std::wstring ANSI_RESET = L"\033[0m";
    std::wstring ANSI_GREEN = L"\033[32m";
    std::wstring ANSI_BLUE = L"\033[34m";

    std::wstring source;
    std::vector<std::wstring> lines;
    const int DEFAULT_MARGIN_LEFT = 6;

    void makeLines();

    void printSourceLine(unsigned int line, std::vector<colorHighlight> colorHighlights);
    std::wstring formattedErrorMessage(ErrorCode errorCode, const std::vector<ErrorMessageArgument>& args, unsigned int line = 0);
    void printLineDivider(unsigned int lineNum);
    void printCaretSupportLine(unsigned int offset);
    void printSquiggleSupportLine(unsigned int lineNum, std::vector<colorHighlight> colorHighlights);
    static std::wstring getTokenValue(Token *token);
    colorHighlight makeTokenHighlight(Token *token, std::wstring color);

    void printWrongTypeError(const WrongTypeError* error);
    void printWrongBinaryOperandTypeError(const WrongBinaryOperandTypes* error);
    void printIndexOutOfBoundsError(const IndexOutOfBounds* error);
    void printNonIntegerIndexError(const NonIntegerIndex* error);
    void printIndexingNonArrayError(const IndexingNonArray* error);

    void printVariableRedeclarationError(const VariableRedeclaration* error);
    void printExpectedXBeforeYError(const ExpectedXBeforeY* error);
    void printExpectedXAfterYError(const ExpectedXAfterY* error);
    void printUninitializedConstError(const UninitializedConst* error);
    void printUndeclaredVariableError(const UndeclaredVariable* error);
    void printConstReassignmentError(const ConstReassignment* error);
    void printInvalidLValue(const InvalidLValue* error);
    void printInvalidCallError(const InvalidCall* error);
    void printTooManyArgumentsError(const TooManyArguments* error);
    void printTooFewArgumentsError(const TooFewArguments* error);
    void printInvalidReturnPositionError(const InvalidReturnPosition* error);
    void printSelfReferencingInitializerError(const SelfReferencingInitializer* error);
    void printInvalidDefaultParameterPositionError(const InvalidDefaultParameterPosition* error);
    void printInvalidDefaultParameterValueError(const InvalidDefaultParameterValue* error);
};


#endif //BAKLAVA_ERRORPRINTER_H
