//
// Created by kemal on 2/22/2024.
//

#include "TestHelpers.h"
#include <memory>
#include "ErrorPrinter.h"
#include "Lexer.h"
#include "Parser.h"
#include "Program.h"
#include "Expression.h"
#include "ExpressionStatement.h"
#include "Resolver.h"
#include "LexerError.h"

std::unique_ptr<Program> parseSource(const std::wstring &source, Interpreter *interpreter) {
    ErrorPrinter printer(source);
    Lexer lexer(source);
    try {
        lexer.tokenize();
    } catch (LexerError &e) {
        printer.printLexerError(&e);
        throw;
    }
    Parser parser(lexer.tokens);
    std::unique_ptr<Program> program;
    try {
        program = parser.parse();
    } catch (ParserError &e) {
        printer.printParserError(&e);
        throw;
    }
    Resolver resolver(interpreter);
    resolver.resolve(program);
    return program;
}

std::unique_ptr<Program> parseSourceNoResolver(const std::wstring &source) {
    ErrorPrinter printer(source);
    Lexer lexer(source);
    try {
        lexer.tokenize();
    } catch (LexerError &e) {
        printer.printLexerError(&e);
        throw;
    }
    Parser parser(lexer.tokens);
    std::unique_ptr<Program> program;
    try {
        program = parser.parse();
    } catch (ParserError &e) {
        printer.printParserError(&e);
        throw;
    }
    return program;
}

// Seems not to be in use anymore
Expression *bypassExpressionStatement(Program *program) {
    if (program->statements.size() != 1) {
        throw std::runtime_error("Expected single expression");
    }
    auto expression = dynamic_cast<Expression *>(((ExpressionStatement *) program->statements[0])->expr);
    if (expression == nullptr) {
        throw std::runtime_error("Expected expression");
    }
    return expression;
}

// Doesn't use resolver, shouldn't be needed?
Expression *parseSingleExpression(const std::wstring &source) {
    std::wstring sourceWithSemicolon = source + L";";
    ErrorPrinter printer(sourceWithSemicolon);

    Lexer lexer(sourceWithSemicolon);
    try{
        lexer.tokenize();
    } catch (LexerError &e) {
        printer.printLexerError(&e);
        throw;
    }

    Parser parser(lexer.tokens);
    std::unique_ptr<Program> program;
    try{
        program = parser.parse();
    }catch(ParserError &e) {
        printer.printParserError(&e);
        throw;
    }

    if (program->statements.size() != 1) {
        throw std::runtime_error("Expected single expression");
    }
    auto expression = dynamic_cast<Expression *>(((ExpressionStatement *) program->statements[0])->expr);
    if (expression == nullptr) {
        throw std::runtime_error("Expected expression");
    }
    return expression;
}

Token *makeToken(TokenType type, const std::wstring &lexeme) {
    return new Token(type, lexeme, 0, 0);
}

void checkToken(const Token *token, TokenType type, const std::wstring &value) {
    CHECK(token->type == type);
    CHECK(token->value == value);
}

void checkToken(const Token *token, TokenType type, const std::wstring &value, unsigned int line,
                unsigned int charIndexOnLine) {
    checkToken(token, type, value);
    CHECK(token->line == line);
    CHECK(token->offset == charIndexOnLine);
}