//
// Created by kemal on 2/4/2024.
//

#include "Parser.h"
#include "Program.h"
#include "BinaryExpression.h"
#include "LogicalExpression.h"
#include "UnaryExpression.h"
#include "BooleanLiteralExpression.h"
#include "NumericLiteralExpression.h"
#include "StringLiteralExpression.h"
#include "NullLiteralExpression.h"
#include "TokenType.h"
#include "GroupingExpression.h"
#include "PrintStatement.h"
#include "ExpressionStatement.h"

#include "ExpectedXBeforeY.h"

std::unique_ptr<Program> Parser::parse() {
    std::unique_ptr<Program> program = std::make_unique<Program>();
    while(!atType(TokenType::Eof)) {
        program->statements.push_back(statement());
    }
    return program;
}

Statement* Parser::statement() {
    if(match({TokenType::Print})){
        return printStatement();
    }
    return expressionStatement();
}

Statement* Parser::printStatement() {
    ExprPtr value = expression();
    if(match({TokenType::Semicolon})){
        return new PrintStatement(value);
    }
    throw ExpectedXBeforeY(L"semicolon", previous(), at());
}

Statement* Parser::expressionStatement() {
    ExprPtr value = expression();
    if(match({TokenType::Semicolon})){
        return new ExpressionStatement(value);
    }
    throw ExpectedXBeforeY(L"semicolon", previous(), at());
}

ExprPtr Parser::expression() {
    return logicalOrExpression();
}

ExprPtr Parser::logicalOrExpression() {
    ExprPtr expr = logicalAndExpression();
    while(match({TokenType::DoublePipe})){
        Token* op = previous();
        ExprPtr right = logicalAndExpression();
        expr = new LogicalExpression(expr, op, right);
    }
    return expr;
}

ExprPtr Parser::logicalAndExpression() {
    ExprPtr expr = equalityExpression();
    while(match({TokenType::DoubleAmpersand})){
        Token* op = previous();
        ExprPtr right = equalityExpression();
        expr = new LogicalExpression(expr, op, right);
    }
    return expr;
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
    if(match({TokenType::OpenParenthesis})){
        ExprPtr expr = expression();
        if(match({TokenType::ClosedParenthesis})){
            return new GroupingExpression(expr);
        }
        throw ExpectedXBeforeY(L"zatvorena zagrada", previous(), at());
    }
    throw ExpectedXBeforeY(L"izraz", previous(), at());
}

bool Parser::match(const std::vector<TokenType>& types) {
    for(auto type : types){
        if(atType(type)){
            advance();
            return true;
        }
    }
    return false;
}
