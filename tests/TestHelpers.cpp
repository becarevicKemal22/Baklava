//
// Created by kemal on 2/22/2024.
//

#include "TestHelpers.h"
#include <memory>
#include "Ast.h"
#include "ErrorPrinter.h"
#include "Lexer.h"
#include "Parser.h"
#include "Program.h"
#include "Expression.h"
#include "ExpressionStatement.h"

std::unique_ptr<Program> parseSource(const std::wstring& source){
    ErrorPrinter printer(source);
    Lexer lexer(source, &printer);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    std::unique_ptr<Program> program = parser.parse();
    return program;
}

// Seems not to be in use anymore
Expression* bypassExpressionStatement(Program* program){
    if(program->statements.size() != 1){
        throw std::runtime_error("Expected single expression");
    }
    auto expression = dynamic_cast<Expression*>(((ExpressionStatement*)program->statements[0])->expr);
    if(expression == nullptr){
        throw std::runtime_error("Expected expression");
    }
    return expression;
}

Expression* parseSingleExpression(const std::wstring& source){
    std::wstring sourceWithSemicolon = source + L";";
    auto program = parseSource(sourceWithSemicolon);
    if(program->statements.size() != 1){
        throw std::runtime_error("Expected single expression");
    }
    auto expression = dynamic_cast<Expression*>(((ExpressionStatement*)program->statements[0])->expr);
    if(expression == nullptr){
        throw std::runtime_error("Expected expression");
    }
    return expression;
}

Token* makeToken(TokenType type, const std::wstring& lexeme){
    return new Token(type, lexeme, 0, 0);
}

void checkToken(const Token* token, TokenType type, const std::wstring& value){
    CHECK(token->type == type);
    CHECK(token->value == value);
}

void checkToken(const Token* token, TokenType type, const std::wstring& value, unsigned int line, unsigned int charIndexOnLine){
    checkToken(token, type, value);
    CHECK(token->line == line);
    CHECK(token->offset == charIndexOnLine);
}