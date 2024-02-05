//
// Created by kemal on 2/4/2024.
//

#include "Parser.h"
#include "Program.h"
#include "BinaryExpression.h"
#include "UnaryExpression.h"
#include "BooleanLiteralExpression.h"
#include "NumericLiteralExpression.h"
#include "StringLiteralExpression.h"
#include "NullLiteralExpression.h"
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

ExprPtr Parser::expression() {
    return equalityExpression();
}

ExprPtr Parser::equalityExpression() {
    ExprPtr expr = comparisonExpression();
    while(match({TokenType::BangEqual, TokenType::DoubleEqual})){
        Token* op = previous();
        ExprPtr right = comparisonExpression();
        expr = new BinaryExpression(expr, op, right);
    }
    return expr;
}

ExprPtr Parser::comparisonExpression() {
    ExprPtr expr = termExpression();
    while(match({TokenType::Greater, TokenType::GreaterEqual, TokenType::Less, TokenType::LessEqual})){
        Token* op = previous();
        ExprPtr right = termExpression();
        expr = new BinaryExpression(expr, op, right);
    }
    return expr;
}

ExprPtr Parser::termExpression() {
    ExprPtr expr = factorExpression();
    while(match({TokenType::Minus, TokenType::Plus})){
        Token* op = previous();
        ExprPtr right = factorExpression();
        expr = new BinaryExpression(expr, op, right);
    }
    return expr;
}

ExprPtr Parser::factorExpression() {
    ExprPtr expr = unaryExpression();
    while(match({TokenType::Slash, TokenType::Star})){
        Token* op = previous();
        ExprPtr right = unaryExpression();
        expr = new BinaryExpression(expr, op, right);
    }
    return expr;
}

ExprPtr Parser::unaryExpression() {
    if(match({TokenType::Bang, TokenType::Minus})){
        Token* op = previous();
        ExprPtr right = unaryExpression();
        return new UnaryExpression(op, right);
    }
    return primaryExpression();
}

ExprPtr Parser::primaryExpression() {
    if(match({TokenType::False})) return new BooleanLiteralExpression(previous());
    if(match({TokenType::True})) return new BooleanLiteralExpression(previous());
    if(match({TokenType::Null})) return new NullLiteralExpression(previous());
    if(match({TokenType::Number})) return new NumericLiteralExpression(previous());
    if(match({TokenType::String})) return new StringLiteralExpression(previous());
    return nullptr;
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