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

std::unique_ptr<Program> parseSource(const std::wstring& source){
    ErrorPrinter printer(source);
    Lexer lexer(source, &printer);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    std::unique_ptr<Program> program = parser.parse();
    return program;
}

Expression* parseSingleExpression(const std::wstring& source){
    auto program = parseSource(source);
    if(program->statements.size() != 1){
        throw std::runtime_error("Expected single expression");
    }
    auto expression = dynamic_cast<Expression*>(program->statements[0]);
    if(expression == nullptr){
        throw std::runtime_error("Expected expression");
    }
    return expression;
}