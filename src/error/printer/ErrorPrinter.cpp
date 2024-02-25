//
// Created by kemal on 1/28/2024.
//

#include <iostream>
#include "ErrorPrinter.h"
#include "ErrorMessages.h"
#include "RuntimeError.h"
#include "WrongTypeError.h"
#include "WrongBinaryOperandTypes.h"

using std::wcout;

const std::wstring ANSI_RED = L"\033[31m";
const std::wstring ANSI_RESET = L"\033[0m";
const std::wstring ANSI_GREEN = L"\033[32m";
const std::wstring ANSI_BLUE = L"\033[34m";

void ErrorPrinter::printErrorMessage(ErrorCode errorCode, const std::vector<ErrorMessageArgument> &args) {
    std::wstring message = formattedErrorMessage(errorCode, args);
    wcout << message << "\n";
}


void ErrorPrinter::printRuntimeError(const RuntimeError *error) {
    if (dynamic_cast<const WrongTypeError *>(error) != nullptr) {
        printWrongTypeError(static_cast<const WrongTypeError *>(error));
    }
    if (dynamic_cast<const WrongBinaryOperandTypes *>(error) != nullptr) {
        printWrongBinaryOperandTypeError(static_cast<const WrongBinaryOperandTypes *>(error));
    }
    std::wcout << "\n";
}

void ErrorPrinter::printWrongTypeError(const WrongTypeError *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line, {
            {
                {error->token->offset, error->token->offset + error->token->value.size() - 1},
                ANSI_RED}
    });
    printSquiggleSupportLine(error->token->line, {
        {{error->token->offset, error->token->offset + error->token->value.size() - 1}, ANSI_RED}});
}

void ErrorPrinter::printWrongBinaryOperandTypeError(const WrongBinaryOperandTypes *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    colorHighlight leftTokenHighlight = {{error->leftToken->offset, error->leftToken->offset + error->leftToken->value.size() - 1}, ANSI_BLUE};
    colorHighlight rightTokenHighlight = {{error->rightToken->offset, error->rightToken->offset + error->rightToken->value.size() - 1}, ANSI_GREEN};
    colorHighlight operatorTokenHighlight = {{error->token->offset, error->token->offset + error->token->value.size() - 1}, ANSI_RED};
    unsigned int leftLine = error->leftToken->line;
    unsigned int rightLine = error->rightToken->line;
    unsigned int operatorLine = error->token->line;
    if(leftLine == rightLine){
        printSourceLine(leftLine, {leftTokenHighlight, operatorTokenHighlight, rightTokenHighlight});
        printSquiggleSupportLine(leftLine, {leftTokenHighlight, operatorTokenHighlight, rightTokenHighlight});
    } else if(leftLine == operatorLine){
        printSourceLine(leftLine, {leftTokenHighlight, operatorTokenHighlight});
        printSquiggleSupportLine(leftLine, {leftTokenHighlight, operatorTokenHighlight});
        printSourceLine(rightLine, {rightTokenHighlight});
        printSquiggleSupportLine(rightLine, {rightTokenHighlight});
    } else if(rightLine == operatorLine){
        printSourceLine(leftLine, {leftTokenHighlight});
        printSquiggleSupportLine(leftLine, {leftTokenHighlight});
        printSourceLine(rightLine, {operatorTokenHighlight, rightTokenHighlight});
        printSquiggleSupportLine(rightLine, {operatorTokenHighlight, rightTokenHighlight});
    } else {
        printSourceLine(leftLine, {leftTokenHighlight});
        printSquiggleSupportLine(leftLine, {leftTokenHighlight});
        printSourceLine(operatorLine, {operatorTokenHighlight});
        printSquiggleSupportLine(operatorLine, {operatorTokenHighlight});
        printSourceLine(rightLine, {rightTokenHighlight});
        printSquiggleSupportLine(rightLine, {rightTokenHighlight});
    }
}

void
ErrorPrinter::printLexerError(ErrorCode errorCode, unsigned int line, unsigned int offset, unsigned int currentChar) {
    std::wstring message;
    if (errorCode == ERROR_UNEXPECTED_CHARACTER) {
        message = formattedErrorMessage(errorCode, {std::wstring(1, source[currentChar]), std::to_string(line),
                                                    std::to_string(offset)});
    } else if (errorCode == ERROR_UNTERMINATED_STRING) {
        message = formattedErrorMessage(errorCode, {std::to_string(line), std::to_string(offset)});
    }
    wcout << message << "\n";
    printSourceLine(line, {{{offset, offset}, ANSI_RED}});
    printCaretSupportLine(offset);
}



// ***********************************************************************
// ----------------------- PRIVATE SUPPORT METHODS -----------------------
// ***********************************************************************



std::wstring ErrorPrinter::formattedErrorMessage(ErrorCode errorCode, const std::vector<ErrorMessageArgument> &args,
                                                 unsigned int line) {
    std::wstring message = L"";
    if (line != 0) {
        message = ANSI_RED + L"Greška na liniji " + std::to_wstring(line) + L": " + ANSI_RESET +
                  ERRORMESSAGES.at(errorCode);
    } else {
        message = ANSI_RED + L"Greška: " + ANSI_RESET + ERRORMESSAGES.at(errorCode);
    }

    for (const auto &arg: args) {
        std::wstring argument;
        if (arg.index() == 0) {
            argument = std::get<0>(arg);
        } else if (arg.index() == 1) {
            std::string s = std::get<1>(arg);
            std::wstring ws(s.begin(), s.end());
            argument = ws;
        } else if (arg.index() == 2) {
            const char *c = std::get<2>(arg);
            std::string s(c);
            std::wstring ws(s.begin(), s.end());
            argument = ws;
        }
        std::wstring placeholder = L"{}";
        size_t pos = message.find(placeholder);
        if (pos != std::string::npos) {
            message.replace(pos, placeholder.length(), argument);
        }
    }
    return message;
}

void ErrorPrinter::printSourceLine(unsigned int lineNumber, std::vector<colorHighlight> colorHighlights) {
    printLineDivider(lineNumber);
    if (colorHighlights.empty()) {
        std::wcout << lines[lineNumber - 1];
        return;
    }
    std::wstring line = lines[lineNumber - 1];
    size_t size = line.size();
    int currentHighlight = 0;
    int startIndex = colorHighlights[currentHighlight].first.first;
    int endIndex = colorHighlights[currentHighlight].first.second;
    std::wstring ANSI_COLOR = colorHighlights[currentHighlight].second;
    for (int i = 0; i < size; i++) {
        if (i == startIndex) {
            wcout << ANSI_COLOR;
        }
        wcout << line[i];
        if (i == endIndex) {
            wcout << ANSI_RESET;
            currentHighlight++;
            if (colorHighlights.size() > currentHighlight) {
                startIndex = colorHighlights[currentHighlight].first.first;
                endIndex = colorHighlights[currentHighlight].first.second;
                ANSI_COLOR = colorHighlights[currentHighlight].second;
            }
        }
    }
    wcout << "\n";
}

void ErrorPrinter::printLineDivider(unsigned int lineNumber = 0) {
    if (lineNumber == 0) {
        wcout << std::wstring(DEFAULT_MARGIN_LEFT, L' ');
    } else {
        std::wstring lineString = std::to_wstring(lineNumber);
        wcout << std::wstring(DEFAULT_MARGIN_LEFT - lineString.length(), L' ') << lineString;
    }
    wcout << " | ";
}

void ErrorPrinter::printCaretSupportLine(unsigned int offset) {
    printLineDivider(0);
    if (offset > 0) {
        std::wcout << ANSI_RED << std::wstring(offset, L'~') << std::wstring(1, L'^') << ANSI_RESET;
        return;
    }
    std::wcout << ANSI_RED << L'^' << ANSI_RESET;
}

void ErrorPrinter::printSquiggleSupportLine(unsigned int lineNum, std::vector<colorHighlight> colorHighlights){
    printLineDivider(0);

    int currentHighlight = 0;
    int index = 0;
    int startIndex = colorHighlights[currentHighlight].first.first;
    int endIndex = colorHighlights[currentHighlight].first.second;
    std::wstring ANSI_COLOR = colorHighlights[currentHighlight].second;
    for(const auto& highlight : colorHighlights){
        if(index > endIndex){
            currentHighlight++;
            startIndex = colorHighlights[currentHighlight].first.first;
            endIndex = colorHighlights[currentHighlight].first.second;
            ANSI_COLOR = colorHighlights[currentHighlight].second;
        }
        if(index < startIndex){
            wcout << std::wstring(startIndex - index, L' ');
            index = startIndex;
        }
        if(ANSI_COLOR == ANSI_RED){
            wcout << ANSI_COLOR << std::wstring(endIndex - index + 1, L'^') << ANSI_RESET;
        } else{
            wcout << ANSI_COLOR << std::wstring(endIndex - index + 1, L'~') << ANSI_RESET;
        }
        index = endIndex + 1;
    }
    wcout << "\n";
}

void ErrorPrinter::makeLines() {
    std::wstring line;
    for (wchar_t c: source) {
        if (c == L'\n') {
            lines.push_back(line);
            line = L"";
        } else {
            line += c;
        }
    }
    lines.push_back(line);
}
