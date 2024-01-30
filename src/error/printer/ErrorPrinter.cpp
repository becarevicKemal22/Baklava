//
// Created by kemal on 1/28/2024.
//

#include <iostream>
#include "ErrorPrinter.h"
#include "ErrorMessages.h"

using std::wcout;

const std::wstring ANSI_RED = L"\033[31m";
const std::wstring ANSI_RESET = L"\033[0m";
const std::wstring ANSI_GREEN = L"\033[32m";
const std::wstring ANSI_BLUE = L"\033[34m";

void ErrorPrinter::printErrorMessage(ErrorCode errorCode, const std::vector<ErrorMessageArgument>& args) {
    std::wstring message = formattedErrorMessage(errorCode, args);
    wcout << message << "\n";
}

void ErrorPrinter::printLexerError(ErrorCode errorCode, unsigned int line, unsigned int offset, unsigned int currentChar) {
    std::wstring message = formattedErrorMessage(errorCode, {std::wstring(1, source[currentChar]), std::to_string(line), std::to_string(offset)});
    wcout << message << "\n";
    printSourceLine(line, {{{offset, offset}, ANSI_RED}});
    printCaretSupportLine(offset);
}

std::wstring ErrorPrinter::formattedErrorMessage(ErrorCode errorCode, const std::vector<ErrorMessageArgument>& args){
    std::wstring message = ANSI_RED + L"Greška: " + ANSI_RESET + ERRORMESSAGES.at(errorCode);

    for (const auto& arg : args) {
        std::wstring argument;
        if (arg.index() == 0) {
            argument = std::get<0>(arg);
        } else if (arg.index() == 1) {
            std::string s = std::get<1>(arg);
            std::wstring ws(s.begin(), s.end());
            argument = ws;
        } else if (arg.index() == 2) {
            const char* c = std::get<2>(arg);
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
    if(colorHighlights.empty()){
        std::wcout << lines[lineNumber - 1];
        return;
    }
    std::wstring line = lines[lineNumber - 1];
    size_t size = line.size();
    int currentHighlight = 0;
    int startIndex = colorHighlights[currentHighlight].first.first;
    int endIndex = colorHighlights[currentHighlight].first.second;
    std::wstring ANSI_COLOR = colorHighlights[currentHighlight].second;
    for(int i = 0; i < size; i++){
        if(i == startIndex){
            wcout << ANSI_COLOR;
        }
        wcout << line[i];
        if(i == endIndex){
            wcout << ANSI_RESET;
            currentHighlight++;
            if(colorHighlights.size() > currentHighlight){
                startIndex = colorHighlights[currentHighlight].first.first;
                endIndex = colorHighlights[currentHighlight].first.second;
                ANSI_COLOR = colorHighlights[currentHighlight].second;
            }
        }
    }
    wcout << "\n";
}

void ErrorPrinter::printLineDivider(unsigned int lineNumber = 0){
    if(lineNumber == 0){
        wcout << std::wstring(DEFAULT_MARGIN_LEFT, L' ');
    }else{
        std::wstring lineString = std::to_wstring(lineNumber);
        wcout << std::wstring(DEFAULT_MARGIN_LEFT - lineString.length(), L' ') << lineString;
    }
    wcout << " | ";
}

void ErrorPrinter::printCaretSupportLine(unsigned int offset){
    printLineDivider(0);
    if(offset > 0){
        std::wcout << ANSI_RED << std::wstring(offset - 1, L'~') << std::wstring(1, L'^') << ANSI_RESET;
        return;
    }
    std::wcout << ANSI_RED << std::wstring(1, L'^') << ANSI_RESET;
}

void ErrorPrinter::makeLines() {
    std::wstring line;
    for (wchar_t c : source) {
        if (c == L'\n') {
            lines.push_back(line);
            line = L"";
        } else {
            line += c;
        }
    }
    lines.push_back(line);
}
