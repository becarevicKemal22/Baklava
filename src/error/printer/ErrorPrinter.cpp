//
// Created by kemal on 1/28/2024.
//

#include <iostream>
#include "ErrorPrinter.h"
#include "ErrorMessages.h"
#include "RuntimeError.h"
#include "WrongTypeError.h"
#include "WrongBinaryOperandTypes.h"
#include "ExpectedXBeforeY.h"
#include "ExpectedXAfterY.h"
#include "UninitializedConst.h"
#include "ParserError.h"
#include "VariableRedeclaration.h"
#include "UndeclaredVariable.h"
#include "ConstReassignment.h"

using std::wcout;

const std::wstring ANSI_RED = L"\033[31m";
const std::wstring ANSI_RESET = L"\033[0m";
const std::wstring ANSI_GREEN = L"\033[32m";
const std::wstring ANSI_BLUE = L"\033[34m";

void ErrorPrinter::printErrorMessage(ErrorCode errorCode, const std::vector<ErrorMessageArgument> &args) {
    std::wstring message = formattedErrorMessage(errorCode, args);
    wcout << message << "\n";
}

// ******************************************************************************
// ----------------------- RUNTIME ERROR PRINTING METHODS -----------------------
// ******************************************************************************

void ErrorPrinter::printRuntimeError(const RuntimeError *error) {
    if (dynamic_cast<const WrongTypeError *>(error) != nullptr) {
        printWrongTypeError(static_cast<const WrongTypeError *>(error));
    }
    if (dynamic_cast<const WrongBinaryOperandTypes *>(error) != nullptr) {
        printWrongBinaryOperandTypeError(static_cast<const WrongBinaryOperandTypes *>(error));
    }
    if (dynamic_cast<const VariableRedeclaration *>(error) != nullptr) {
        printVariableRedeclarationError(static_cast<const VariableRedeclaration *>(error));
    }
    if(dynamic_cast<const UndeclaredVariable*>(error) != nullptr){
        printUndeclaredVariableError(static_cast<const UndeclaredVariable*>(error));
    }
    if(dynamic_cast<const ConstReassignment*>(error) != nullptr){
        printConstReassignmentError(static_cast<const ConstReassignment*>(error));
    }
    if(dynamic_cast<const InvalidCall*>(error) != nullptr){
        printInvalidCallError(static_cast<const InvalidCall*>(error));
    }
    if(dynamic_cast<const InvalidArgumentCount*>(error) != nullptr){
        printInvalidArgumentCountError(static_cast<const InvalidArgumentCount*>(error));
    }
    if(dynamic_cast<const IndexOutOfBounds*>(error) != nullptr){
        printIndexOutOfBoundsError(static_cast<const IndexOutOfBounds*>(error));
    }
    if(dynamic_cast<const NonIntegerIndex*>(error) != nullptr){
        printNonIntegerIndexError(static_cast<const NonIntegerIndex*>(error));
    }
    if(dynamic_cast<const IndexingNonArray*>(error) != nullptr){
        printIndexingNonArrayError(static_cast<const IndexingNonArray*>(error));
    }
    std::wcout << "\n";
}

void ErrorPrinter::printConstReassignmentError(const ConstReassignment *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->name->line);
    wcout << message << "\n";
    printSourceLine(error->name->line, {{ {error->name->offset, error->name->offset + getTokenValue(error->name).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->name->line, {{ {error->name->offset, error->name->offset + getTokenValue(error->name).size() - 1}, ANSI_RED}});
}

void ErrorPrinter::printVariableRedeclarationError(const VariableRedeclaration *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line, {{ {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->token->line, {{ {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
}

void ErrorPrinter::printUndeclaredVariableError(const UndeclaredVariable *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->name->line);
    wcout << message << "\n";
    printSourceLine(error->name->line, {{ {error->name->offset, error->name->offset + getTokenValue(error->name).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->name->line, {{ {error->name->offset, error->name->offset + getTokenValue(error->name).size() - 1}, ANSI_RED}});
}

void ErrorPrinter::printWrongTypeError(const WrongTypeError *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line, {
            {
                    {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                    ANSI_RED}
    });
    printSquiggleSupportLine(error->token->line, {
            {{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
}

void ErrorPrinter::printWrongBinaryOperandTypeError(const WrongBinaryOperandTypes *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    colorHighlight leftTokenHighlight = {{error->leftToken->offset, error->leftToken->offset + getTokenValue(error->leftToken).size() - 1}, ANSI_BLUE};
    colorHighlight rightTokenHighlight = {{error->rightToken->offset, error->rightToken->offset + getTokenValue(error->rightToken).size() - 1}, ANSI_GREEN};
    colorHighlight operatorTokenHighlight = {{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED};
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

void ErrorPrinter::printInvalidCallError(const InvalidCall *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->name->line);
    wcout << message << "\n";
    printSourceLine(error->name->line, {{ {error->name->offset, error->name->offset + getTokenValue(error->name).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->name->line, {{ {error->name->offset, error->name->offset + getTokenValue(error->name).size() - 1}, ANSI_RED}});
}

void ErrorPrinter::printInvalidArgumentCountError(const InvalidArgumentCount *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line, {{ {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_BLUE},
                                         { {error->paren->offset, error->paren->offset + getTokenValue(error->paren).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->token->line, {{ {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_BLUE},
                                                  { {error->paren->offset, error->paren->offset + getTokenValue(error->paren).size() - 1}, ANSI_RED}});
}

void ErrorPrinter::printIndexOutOfBoundsError(const IndexOutOfBounds *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, getMostRelevantToken(error->index)->line);
    wcout << message << "\n";
    auto indexToken = getMostRelevantToken(error->index);
    printSourceLine(indexToken->line, {{ {indexToken->offset, indexToken->offset + getTokenValue(indexToken).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(indexToken->line, {{ {indexToken->offset, indexToken->offset + getTokenValue(indexToken).size() - 1}, ANSI_RED}});
}

void ErrorPrinter::printNonIntegerIndexError(const NonIntegerIndex *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, getMostRelevantToken(error->index)->line);
    wcout << message << "\n";
    auto indexToken = getMostRelevantToken(error->index);
    printSourceLine(indexToken->line, {{ {indexToken->offset, indexToken->offset + getTokenValue(indexToken).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(indexToken->line, {{ {indexToken->offset, indexToken->offset + getTokenValue(indexToken).size() - 1}, ANSI_RED}});

}

void ErrorPrinter::printIndexingNonArrayError(const IndexingNonArray *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line, {{ {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->token->line, {{ {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
}

// ***********************************************************************
// ----------------------- PARSER ERROR PRINTING METHODS -----------------------
// ***********************************************************************

void ErrorPrinter::printParserError(const ParserError *error) {
    if(dynamic_cast<const ExpectedXBeforeY*>(error) != nullptr){
        printExpectedXBeforeYError(static_cast<const ExpectedXBeforeY*>(error));
    }
    else if(dynamic_cast<const ExpectedXAfterY*>(error) != nullptr){
        printExpectedXAfterYError(static_cast<const ExpectedXAfterY*>(error));
    }
    else if(dynamic_cast<const UninitializedConst*>(error) != nullptr){
        printUninitializedConstError(static_cast<const UninitializedConst*>(error));
    }
    else if(dynamic_cast<const InvalidLValue*>(error) != nullptr){
        printInvalidLValue(static_cast<const InvalidLValue*>(error));
    }
    else if(dynamic_cast<const InvalidReturnPosition*>(error) != nullptr){
        printInvalidReturnPositionError(static_cast<const InvalidReturnPosition*>(error));
    }
    else if(dynamic_cast<const SelfReferencingInitializer*>(error) != nullptr) {
        printSelfReferencingInitializerError(static_cast<const SelfReferencingInitializer *>(error));
    }
    else{ // Generic parser error, ne moze se koristiti ni sa cim drugim, jer samo bazni ima ovaj myToken atribut
        std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->myToken->line);
        wcout << message << "\n";
        printSourceLine(error->myToken->line, {{ {error->myToken->offset, error->myToken->offset + getTokenValue(error->myToken).size() - 1}, ANSI_RED}});
        printSquiggleSupportLine(error->myToken->line, {{ {error->myToken->offset, error->myToken->offset + getTokenValue(error->myToken).size() - 1}, ANSI_RED}});
    }
    std::wcout << "\n";
}

void ErrorPrinter::printExpectedXBeforeYError(const ExpectedXBeforeY *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->found->line);
    wcout << message << "\n";
    TokenPtr found = error->found;
    TokenPtr before = error->before;

    if(found->line == before->line){
        printSourceLine(found->line, {makeTokenHighlight(before, ANSI_RED)});

        printLineDivider(0);
        unsigned int missingLocation = found->offset + found->value.size();
        std::wcout << std::wstring(missingLocation, L' ') << ANSI_GREEN << std::wstring(1, L'^') << ANSI_RESET;
        if(before->offset != missingLocation){
            std::wcout << std::wstring(before->offset - missingLocation - 1, L' ');
            std::wcout << ANSI_RED << std::wstring(before->value.size(), L'~');
        }
        std::wcout << ANSI_RESET << "\n";
        printLineDivider(0);
        std::wcout << std::wstring(missingLocation, L' ') << ANSI_GREEN << error->expectedWhat << ANSI_RESET << "\n";
    } else{
        printSourceLine(found->line, {});
        std::wcout << "\n";
        printLineDivider(0);
        unsigned int missingLocation = found->offset + found->value.size();
        std::wcout << std::wstring(missingLocation, L' ') << ANSI_GREEN << std::wstring(1, L'^') << ANSI_RESET << "\n";
        printLineDivider(0);
        std::wcout << std::wstring(missingLocation, L' ') << ANSI_GREEN << error->expectedWhat << ANSI_RESET << "\n";
        printSourceLine(before->line, {makeTokenHighlight(before, ANSI_RED)});
        printLineDivider(0);
        std::wcout << std::wstring(before->offset, L' ');
        std::wcout << ANSI_RED << std::wstring(before->value.size(), L'~');
        std::wcout << ANSI_RESET << "\n";
    }
}

//currently not used for any errors
void ErrorPrinter::printExpectedXAfterYError(const ExpectedXAfterY *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->found->line);
    wcout << message << "\n";
//    TokenPtr found = error->found;
//    TokenPtr after = error->after;
//
//    if(found->line == after->line){
//        printSourceLine(found->line, {makeTokenHighlight(after, ANSI_RED)});
//
//        printLineDivider(0);
//        unsigned int missingLocation = found->offset + found->value.size();
//        std::wcout << std::wstring(missingLocation, L' ') << ANSI_GREEN << std::wstring(1, L'^') << ANSI_RESET;
//        if(after->offset != missingLocation){
//            std::wcout << std::wstring(after->offset - missingLocation - 1, L' ');
//            std::wcout << ANSI_RED << std::wstring(after->value.size(), L'~');
//        }
//        std::wcout << ANSI_RESET << "\n";
//        printLineDivider(0);
//        std::wcout << std::wstring(missingLocation, L' ') << ANSI_GREEN << error->expectedWhat << ANSI_RESET << "\n";
//    } else{
//        printSourceLine(found->line, {});
//        std::wcout << "\n";
//        printLineDivider(0);
//        unsigned int missingLocation = found->offset + found->value.size();
//        std::wcout << std::wstring(missingLocation, L' ') << ANSI_GREEN << std::wstring(1, L'^') << ANSI_RESET << "\n";
//        printLineDivider(0);
//        std::wcout << std::wstring(missingLocation, L' ') << ANSI_GREEN << error->expectedWhat << ANSI_RESET << "\n";
//        printSourceLine(after->line, {makeTokenHighlight(after, ANSI_RED)});
//        printLineDivider(0);
//        std::wcout << std::wstring(after->offset, L' ');
//        std::wcout << ANSI_RED << std::wstring(after->value.size(), L'~');
//        std::wcout << ANSI_RESET << "\n";
//    }

    std::wcout << "AFTER Y! Nije jos implementiran highlighthing \n";
}

void ErrorPrinter::printUninitializedConstError(const UninitializedConst *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->identifier->line);
    wcout << message << "\n";
    printSourceLine(error->identifier->line, {{ {error->identifier->offset, error->identifier->offset + getTokenValue(error->identifier).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->identifier->line, {{ {error->identifier->offset, error->identifier->offset + getTokenValue(error->identifier).size() - 1}, ANSI_RED}});
}

void ErrorPrinter::printInvalidLValue(const InvalidLValue *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line, {{ {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->token->line, {{ {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
}

void ErrorPrinter::printInvalidReturnPositionError(const InvalidReturnPosition *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line, {{ {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->token->line, {{ {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
}

void ErrorPrinter::printSelfReferencingInitializerError(const SelfReferencingInitializer *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line, {{ {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->token->line, {{ {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
}

// ***********************************************************************
// ----------------------- LEXER ERROR PRINTING METHODS -----------------------
// ***********************************************************************

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

std::wstring ErrorPrinter::getTokenValue(Token *token) {
    if(token->type == TokenType::String){
        return L"\"" + token->value + L"\"";
    }
    return token->value;
}

colorHighlight ErrorPrinter::makeTokenHighlight(Token *token, std::wstring color) {
    return {{token->offset, token->offset + token->value.size() - 1}, color};
}