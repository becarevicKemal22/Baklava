//
// Created by kemal on 2/4/2024.
//

#include "Parser.h"
#include "Program.h"
#include "BinaryExpression.h"
#include "TokenType.h"

std::unique_ptr<Program> Parser::parse() {
    std::unique_ptr<Program> program = std::make_unique<Program>();
    while(!atType(TokenType::Eof)) {
        program->statements.push_back(statement());
    }
    return program;
}

Statement* Parser::statement() {
    return expression();
}

Statement* Parser::expression() {
    return equalityExpression();
}

Statement* Parser::equalityExpression() {
    Statement* expr = comparisonExpression();
    while(match({TokenType::BangEqual, TokenType::DoubleEqual})){
        Token op = previous();
        Statement* right = comparisonExpression();
        expr = new BinaryExpression(expr, op, right);
    }
    return expr;
}

bool Parser::match(std::vector<TokenType> types) {
    for(auto type : types){
        if(atType(type)){
            advance();
            return true;
        }
    }
    return false;
}