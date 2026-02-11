//
// Created by kemal on 1/28/2024.
//

#include <iostream>
#include <cstdlib>

#include "ErrorPrinter.h"
#include "ErrorMessages.h"
#include "RuntimeError.h"
#include "WrongTypeError.h"
#include "WrongBinaryOperandTypes.h"
#include "ExpectedXBeforeY.h"
#include "ExpectedXAfterY.h"
#include "UninitializedConst.h"
#include "ParserError.h"
#include "IdentifierRedeclaration.h"
#include "UndeclaredIdentifier.h"
#include "ConstReassignment.h"
#include "InvalidDefaultParameterPosition.h"
#include "InvalidDefaultParameterValue.h"

using std::wcout;

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
    else if (dynamic_cast<const WrongBinaryOperandTypes *>(error) != nullptr) {
        printWrongBinaryOperandTypeError(static_cast<const WrongBinaryOperandTypes *>(error));
    }
    else if (dynamic_cast<const IdentifierRedeclaration *>(error) != nullptr) {
        printIdentifierRedeclarationError(static_cast<const IdentifierRedeclaration *>(error));
    }
    else if (dynamic_cast<const UndeclaredIdentifier *>(error) != nullptr) {
        printUndeclaredIdentifierError(static_cast<const UndeclaredIdentifier *>(error));
    }
    else if (dynamic_cast<const ConstReassignment *>(error) != nullptr) {
        printConstReassignmentError(static_cast<const ConstReassignment *>(error));
    }
    else if (dynamic_cast<const InvalidCall *>(error) != nullptr) {
        printInvalidCallError(static_cast<const InvalidCall *>(error));
    }
    else if (dynamic_cast<const TooManyArguments *>(error) != nullptr) {
        printTooManyArgumentsError(static_cast<const TooManyArguments *>(error));
    }
    else if (dynamic_cast<const TooFewArguments *>(error) != nullptr) {
        printTooFewArgumentsError(static_cast<const TooFewArguments *>(error));
    }
    else if (dynamic_cast<const IndexOutOfBounds *>(error) != nullptr) {
        printIndexOutOfBoundsError(static_cast<const IndexOutOfBounds *>(error));
    }
    else if (dynamic_cast<const NonIntegerIndex *>(error) != nullptr) {
        printNonIntegerIndexError(static_cast<const NonIntegerIndex *>(error));
    }
    else if (dynamic_cast<const IndexingNonArray *>(error) != nullptr) {
        printIndexingNonArrayError(static_cast<const IndexingNonArray *>(error));
    }
    else if (dynamic_cast<const WrongTypeToStatement *>(error) != nullptr) {
        printWrongTypeToStatementError(static_cast<const WrongTypeToStatement *>(error));
    }
    else if (dynamic_cast<const ConstructorNoNew *>(error) != nullptr) {
        printConstructorNoNewError(static_cast<const ConstructorNoNew *>(error));
    } else if (dynamic_cast<const ClassNotFound *>(error) != nullptr) {
        printClassNotFoundError(static_cast<const ClassNotFound *>(error));
    } else if (dynamic_cast<const InvalidPropertyAccess*>(error) != nullptr) {
        printInvalidPropertyAccessError(static_cast<const InvalidPropertyAccess*>(error));
    } else if (dynamic_cast<const ObjHasNoAttr*>(error) != nullptr) {
        printObjHasNoAttrError(static_cast<const ObjHasNoAttr*>(error));
    } else {
        wcout << L"INTERNAL ERROR: UNKNOWN RUNTIME ERROR TYPE\n";
    }
    wcout << "\n";
}

void ErrorPrinter::printConstReassignmentError(const ConstReassignment *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->name->line);
    wcout << message << "\n";
    printSourceLine(error->name->line,
                    {{{error->name->offset, error->name->offset + getTokenValue(error->name).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->name->line,
                             {{{error->name->offset, error->name->offset + getTokenValue(error->name).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printIdentifierRedeclarationError(const IdentifierRedeclaration *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                      ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printUndeclaredIdentifierError(const UndeclaredIdentifier *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->name->line);
    wcout << message << "\n";
    printSourceLine(error->name->line,
                    {{{error->name->offset, error->name->offset + getTokenValue(error->name).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->name->line,
                             {{{error->name->offset, error->name->offset + getTokenValue(error->name).size() - 1},
                               ANSI_RED}});
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
    colorHighlight leftTokenHighlight = {
            {error->leftToken->offset, error->leftToken->offset + getTokenValue(error->leftToken).size() - 1},
            ANSI_BLUE};
    colorHighlight rightTokenHighlight = {
            {error->rightToken->offset, error->rightToken->offset + getTokenValue(error->rightToken).size() - 1},
            ANSI_GREEN};
    colorHighlight operatorTokenHighlight = {
            {error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED};

    unsigned int leftLine = error->leftToken->line;
    unsigned int operatorLine = error->token->line;
    unsigned int rightLine = error->rightToken->line;

    std::vector<colorHighlight> highlights = {leftTokenHighlight, operatorTokenHighlight, rightTokenHighlight};
    unsigned int tokenLines[3] = {leftLine, operatorLine, rightLine};

    int i = 0;
    int consecutive = 1;

    std::vector<colorHighlight> sameLineHighlights;
    while (i < 3) {
        if (tokenLines[i] != 0) {
            break;
        }
        i++;
    }
    sameLineHighlights.push_back(highlights[i]); // first non-zero line (valid) highlight token
    i++;

    unsigned int currLine = tokenLines[i - 1];
    while (i < 3) {
        if (tokenLines[i] == 0) {
            i++;
            continue;
        }
        if (tokenLines[i] == currLine) {
            sameLineHighlights.push_back(highlights[i]);
            consecutive++;
        } else {
            if (consecutive > 0) {
                printSourceLine(currLine, sameLineHighlights);
                printSquiggleSupportLine(currLine, sameLineHighlights);
            }
            sameLineHighlights.clear();
            sameLineHighlights.push_back(highlights[i]);
            currLine = tokenLines[i];
            consecutive = 1;
        }
        i++;
    }
    if (consecutive > 0) {
        printSourceLine(currLine, sameLineHighlights);
        printSquiggleSupportLine(currLine, sameLineHighlights);
    }
}

void ErrorPrinter::printInvalidCallError(const InvalidCall *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->name->line);
    wcout << message << "\n";
    printSourceLine(error->name->line,
                    {{{error->name->offset, error->name->offset + getTokenValue(error->name).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->name->line,
                             {{{error->name->offset, error->name->offset + getTokenValue(error->name).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printTooManyArgumentsError(const TooManyArguments *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    unsigned int tokenLine = error->token->line;
    unsigned int parenLine = error->paren->line;
    if (tokenLine == parenLine) {
        printSourceLine(tokenLine,
                        {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_BLUE},
                         {{error->paren->offset, error->paren->offset + getTokenValue(error->paren).size() -
                                                 1},                                                             ANSI_RED}});
        printSquiggleSupportLine(tokenLine, {{{error->token->offset,
                                                                              error->token->offset +
                                                                              getTokenValue(error->token).size() -
                                                                              1}, ANSI_BLUE},
                                                      {{error->paren->offset, error->paren->offset +
                                                                              getTokenValue(error->paren).size() -
                                                                              1}, ANSI_RED}});
    } else {
        printSourceLine(tokenLine, {{{error->token->offset, error->token->offset +
                                                                      getTokenValue(error->token).size() - 1}, ANSI_BLUE}});
        printSquiggleSupportLine(tokenLine, {{{error->token->offset, error->token->offset +
                                                                      getTokenValue(error->token).size() - 1}, ANSI_BLUE}});
        printSourceLine(parenLine, {{{error->paren->offset, error->paren->offset +
                                                                      getTokenValue(error->paren).size() - 1}, ANSI_RED}});
        printSquiggleSupportLine(parenLine, {{{error->paren->offset, error->paren->offset +
                                                                      getTokenValue(error->paren).size() - 1}, ANSI_RED}});
    }
}

void ErrorPrinter::printTooFewArgumentsError(const TooFewArguments *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    unsigned int tokenLine = error->token->line;
    unsigned int parenLine = error->paren->line;
    if (tokenLine == parenLine) {
        printSourceLine(tokenLine,
                        {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_BLUE},
                         {{error->paren->offset, error->paren->offset + getTokenValue(error->paren).size() -
                                                 1},                                                             ANSI_RED}});
        printSquiggleSupportLine(tokenLine, {{{error->token->offset,
                                                                              error->token->offset +
                                                                              getTokenValue(error->token).size() -
                                                                              1}, ANSI_BLUE},
                                                      {{error->paren->offset, error->paren->offset +
                                                                              getTokenValue(error->paren).size() -
                                                                              1}, ANSI_RED}});
    } else {
        printSourceLine(tokenLine, {{{error->token->offset, error->token->offset +
                                                                      getTokenValue(error->token).size() - 1}, ANSI_BLUE}});
        printSquiggleSupportLine(tokenLine, {{{error->token->offset, error->token->offset +
                                                                      getTokenValue(error->token).size() - 1}, ANSI_BLUE}});
        printSourceLine(parenLine, {{{error->paren->offset, error->paren->offset +
                                                                      getTokenValue(error->paren).size() - 1}, ANSI_RED}});
        printSquiggleSupportLine(parenLine, {{{error->paren->offset, error->paren->offset +
                                                                      getTokenValue(error->paren).size() - 1}, ANSI_RED}});
    }
}

void ErrorPrinter::printIndexOutOfBoundsError(const IndexOutOfBounds *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments,
                                                 getMostRelevantToken(error->index)->line);
    wcout << message << "\n";
    auto indexToken = getMostRelevantToken(error->index);
    printSourceLine(indexToken->line,
                    {{{indexToken->offset, indexToken->offset + getTokenValue(indexToken).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(indexToken->line,
                             {{{indexToken->offset, indexToken->offset + getTokenValue(indexToken).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printNonIntegerIndexError(const NonIntegerIndex *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments,
                                                 getMostRelevantToken(error->index)->line);
    wcout << message << "\n";
    auto indexToken = getMostRelevantToken(error->index);
    printSourceLine(indexToken->line,
                    {{{indexToken->offset, indexToken->offset + getTokenValue(indexToken).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(indexToken->line,
                             {{{indexToken->offset, indexToken->offset + getTokenValue(indexToken).size() - 1},
                               ANSI_RED}});

}

void ErrorPrinter::printIndexingNonArrayError(const IndexingNonArray *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                      ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printWrongTypeToStatementError(const WrongTypeToStatement *error) {
    wcout << formattedErrorMessage(error->code, error->messageArguments, error->keyword->line) << "\n";

    unsigned int keywordLine = error->keyword->line;
    unsigned int tokenLine = error->token->line;
    if (keywordLine == tokenLine) {
        printSourceLine(error->keyword->line, {{{error->keyword->offset, error->keyword->offset +
                                                                      getTokenValue(error->keyword).size() - 1},
                                               ANSI_BLUE}, {{error->token->offset, error->token->offset +
                                                               getTokenValue(error->token).size() - 1}, ANSI_RED}});
        printSquiggleSupportLine(error->keyword->line, {{{error->keyword->offset, error->keyword->offset +
                                                                          getTokenValue(error->keyword).size() - 1},
                                                       ANSI_BLUE}, {{error->token->offset, error->token->offset +
                                                                       getTokenValue(error->token).size() - 1}, ANSI_RED}});
    }else {
        printSourceLine(error->keyword->line, {{{error->keyword->offset, error->keyword->offset +
                                                                      getTokenValue(error->keyword).size() - 1},
                                               ANSI_BLUE}});
        printSquiggleSupportLine(error->keyword->line, {{{error->keyword->offset, error->keyword->offset +
                                                                          getTokenValue(error->keyword).size() - 1},
                                                       ANSI_BLUE}});
        printSourceLine(error->token->line, {{{error->token->offset, error->token->offset +
                                                                       getTokenValue(error->token).size() - 1}, ANSI_RED}});
        printSquiggleSupportLine(error->keyword->line, {{{error->token->offset, error->token->offset +
                                                                       getTokenValue(error->token).size() - 1}, ANSI_RED}});
    }
}

void ErrorPrinter::printConstructorNoNewError(const ConstructorNoNew *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printClassNotFoundError(const ClassNotFound *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printInvalidPropertyAccessError(const InvalidPropertyAccess *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printObjHasNoAttrError(const ObjHasNoAttr *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1}, ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}



// ***********************************************************************
// ----------------------- PARSER ERROR PRINTING METHODS -----------------
// ***********************************************************************


void ErrorPrinter::printParserError(const ParserError *error) {
    if (dynamic_cast<const ExpectedXBeforeY *>(error) != nullptr) {
        printExpectedXBeforeYError(static_cast<const ExpectedXBeforeY *>(error));
    } else if (dynamic_cast<const ExpectedXAfterY *>(error) != nullptr) {
        printExpectedXAfterYError(static_cast<const ExpectedXAfterY *>(error));
    } else if (dynamic_cast<const UninitializedConst *>(error) != nullptr) {
        printUninitializedConstError(static_cast<const UninitializedConst *>(error));
    } else if (dynamic_cast<const InvalidLValue *>(error) != nullptr) {
        printInvalidLValue(static_cast<const InvalidLValue *>(error));
    } else if (dynamic_cast<const InvalidReturnPosition *>(error) != nullptr) {
        printInvalidReturnPositionError(static_cast<const InvalidReturnPosition *>(error));
    } else if (dynamic_cast<const SelfReferencingInitializer *>(error) != nullptr) {
        printSelfReferencingInitializerError(static_cast<const SelfReferencingInitializer *>(error));
    } else if (dynamic_cast<const InvalidDefaultParameterPosition *>(error) != nullptr) {
        printInvalidDefaultParameterPositionError(static_cast<const InvalidDefaultParameterPosition *>(error));
    } else if (dynamic_cast<const InvalidDefaultParameterValue *>(error) != nullptr) {
        printInvalidDefaultParameterValueError(static_cast<const InvalidDefaultParameterValue *>(error));
    } else if (dynamic_cast<const InvalidForLoopStep *>(error) != nullptr) {
        printInvalidForLoopStepError(static_cast<const InvalidForLoopStep *>(error));

    } else if (dynamic_cast<const InvalidNew *>(error) != nullptr) {
        printInvalidNewError(static_cast<const InvalidNew *>(error));
    } else if (dynamic_cast<const InvalidThisPosition *>(error) != nullptr) {
        printInvalidThisPositionError(static_cast<const InvalidThisPosition *>(error));
    } else { // Generic parser error, ne moze se koristiti ni sa cim drugim, jer samo bazni ima ovaj myToken atribut
        std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->myToken->line);
        wcout << message << "\n";
        printSourceLine(error->myToken->line,
                        {{{error->myToken->offset, error->myToken->offset + getTokenValue(error->myToken).size() - 1},
                          ANSI_RED}});
        printSquiggleSupportLine(error->myToken->line, {{{error->myToken->offset, error->myToken->offset +
                                                                                  getTokenValue(error->myToken).size() -
                                                                                  1}, ANSI_RED}});
    }
    std::wcout << "\n";
}

void ErrorPrinter::printExpectedXBeforeYError(const ExpectedXBeforeY *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->found->line);
    wcout << message << "\n";
    TokenPtr found = error->found;
    TokenPtr before = error->before;

    if (found->line == before->line) {
        printSourceLine(found->line, {makeTokenHighlight(before, ANSI_RED)});

        printLineDivider(0);
        unsigned int missingLocation = found->offset + found->value.size();
        std::wcout << std::wstring(missingLocation, L' ') << ANSI_GREEN << std::wstring(1, L'^') << ANSI_RESET;
        if (before->offset != missingLocation) {
            std::wcout << std::wstring(before->offset - missingLocation - 1, L' ');
            std::wcout << ANSI_RED << std::wstring(before->value.size(), L'~');
        }
        std::wcout << ANSI_RESET << "\n";
        printLineDivider(0);
        std::wcout << std::wstring(missingLocation, L' ') << ANSI_GREEN << error->expectedWhat << ANSI_RESET << "\n";
    } else {
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
    printSourceLine(error->identifier->line, {{{error->identifier->offset,
                                                error->identifier->offset + getTokenValue(error->identifier).size() -
                                                1}, ANSI_RED}});
    printSquiggleSupportLine(error->identifier->line, {{{error->identifier->offset, error->identifier->offset +
                                                                                    getTokenValue(
                                                                                            error->identifier).size() -
                                                                                    1}, ANSI_RED}});
}

void ErrorPrinter::printInvalidLValue(const InvalidLValue *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                      ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printInvalidReturnPositionError(const InvalidReturnPosition *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                      ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printInvalidThisPositionError(const InvalidThisPosition *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                      ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printSelfReferencingInitializerError(const SelfReferencingInitializer *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                      ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printInvalidDefaultParameterPositionError(const InvalidDefaultParameterPosition *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                      ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printInvalidDefaultParameterValueError(const InvalidDefaultParameterValue *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                      ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printInvalidForLoopStepError(const InvalidForLoopStep *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                      ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}

void ErrorPrinter::printInvalidNewError(const InvalidNew *error) {
    std::wstring message = formattedErrorMessage(error->code, error->messageArguments, error->token->line);
    wcout << message << "\n";
    printSourceLine(error->token->line,
                    {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                      ANSI_RED}});
    printSquiggleSupportLine(error->token->line,
                             {{{error->token->offset, error->token->offset + getTokenValue(error->token).size() - 1},
                               ANSI_RED}});
}


// ***********************************************************************
// ----------------------- LEXER ERROR PRINTING METHODS ------------------
// ***********************************************************************

void ErrorPrinter::printUnexpectedCharacterError(const UnexpectedCharacter *error) {
    wcout << formattedErrorMessage(error->code, error->messageArguments) << "\n";
    printSourceLine(error->line, {{{error->column, error->column}, ANSI_RED}});
    printCaretSupportLine(error->column);
}

void ErrorPrinter::printUnterminatedStringError(const UnterminatedString *error) {
    wcout << formattedErrorMessage(error->code, error->messageArguments) << "\n";
    printSourceLine(error->line, {{{error->column, error->column}, ANSI_RED}});
    printCaretSupportLine(error->column);
}

void
ErrorPrinter::printLexerError(const LexerError *error) {
    if (dynamic_cast<const UnexpectedCharacter *>(error) != nullptr) {
        printUnexpectedCharacterError(static_cast<const UnexpectedCharacter *>(error));
    } else if (dynamic_cast<const UnterminatedString *>(error) != nullptr) {
        printUnterminatedStringError(static_cast<const UnterminatedString *>(error));
    } else {
        throw "INTERNAL ERROR: Invalid lexer error type!";
    }
    std::wcout << "\n";
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
        if (i >=
            endIndex) { // Ovdje stoji sada >= a prije je bilo samo ==. To sam stavio zbog situacije sa operatorom kod inkrementa for petlji, gdje operator ne postoji pa ga postavim ovako na indeks identifiera. Posto im je isti offset onda nakon sto zavrsi prvi highlight, ustvari nikad ne pocne i ne zavrsi drugi (highlight operatora), tako da bolje mozda da stoji ovako >= da bi mogao zavrsiti operator i preci na iduci. Sto se tice for petlje ne bi trebalo praviti nikakve probleme, jer se nikad odmah iza operatora ne nalazi inkrement. S druge strane nemam pojma da li ce ovo neku drugu poruku zeznuti, tako da ako zezne, ovdje na ovom komentaru predlazem da se vrati na == i da se gore u wrongBinaryType error onda bas sve kombinacije provjere da li je linija operatora 0 linija desnog linija lijevog i sve tako redom da se izvrti tako da se sigurno provjeri koji.
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

void ErrorPrinter::printSquiggleSupportLine(unsigned int lineNum, std::vector<colorHighlight> colorHighlights) {
    printLineDivider(0);

    int currentHighlight = 0;
    int index = 0;
    int startIndex = colorHighlights[currentHighlight].first.first;
    int endIndex = colorHighlights[currentHighlight].first.second;
    std::wstring ANSI_COLOR = colorHighlights[currentHighlight].second;
    for (const auto &highlight: colorHighlights) {
        if (index > endIndex) {
            currentHighlight++;
            startIndex = colorHighlights[currentHighlight].first.first;
            endIndex = colorHighlights[currentHighlight].first.second;
            ANSI_COLOR = colorHighlights[currentHighlight].second;
        }
        if (index < startIndex) {
            wcout << std::wstring(startIndex - index, L' ');
            index = startIndex;
        }
        if (ANSI_COLOR == ANSI_RED) {
            wcout << ANSI_COLOR << std::wstring(endIndex - index + 1, L'^') << ANSI_RESET;
        } else {
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
    if (token->type == TokenType::String) {
        return L"\"" + token->value + L"\"";
    }
    return token->value;
}

colorHighlight ErrorPrinter::makeTokenHighlight(Token *token, std::wstring color) {
    return {{token->offset, token->offset + token->value.size() - 1}, color};
}