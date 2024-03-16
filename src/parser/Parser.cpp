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
#include "VarDeclarationStatement.h"
#include "VariableExpression.h"
#include "UninitializedConst.h"

#include "ExpectedXBeforeY.h"
#include "AssignmentExpression.h"
#include "InvalidLValue.h"
#include "ExceptionHelpers.h"
#include "BlockStatement.h"
#include "IfStatement.h"

std::unique_ptr<Program> Parser::parse() {
    std::unique_ptr<Program> program = std::make_unique<Program>();
    while(!atType(TokenType::Eof)) {
        program->statements.push_back(declaration());
    }
    return program;
}

Statement* Parser::declaration() {
    if(match({TokenType::Var}) || match({TokenType::Const})){
        return varDeclarationStatement();
    }
    return statement();
}

Statement* Parser::varDeclarationStatement() {
    bool isConst = previous()->type == TokenType::Const;
    if(isConst){
        if(!match({TokenType::Var})){
            throw ExpectedXBeforeY(L"var", previous(), at());
        }
    }
    if(!atType(TokenType::Identifier)){
        throw ExpectedXBeforeY(L"identifikator", previous(), at());
    }
    advance();
    Token* name = previous();
    ExprPtr initializer = nullptr;
    if(match({TokenType::Equal})){
        initializer = expression();
    }
    if(isConst && !initializer){
        throw UninitializedConst(name);
    }
    if(match({TokenType::Semicolon})){
        return new VarDeclarationStatement(name, initializer, isConst);
    }
    throw ExpectedXBeforeY(L";", previous(), at());
}

Statement* Parser::statement() {
    if(match({TokenType::Print})){
        return printStatement();
    }
    if(match({TokenType::OpenBrace})){
        return new BlockStatement(block());
    }
    if(match({TokenType::If})){
        return ifStatement();
    }
    return expressionStatement();
}

Statement* Parser::printStatement() {
    ExprPtr value = expression();
    if(match({TokenType::Semicolon})){
        return new PrintStatement(value);
    }
    throw ExpectedXBeforeY(L";", previous(), at());
}

std::vector<Statement*> Parser::block() {
    std::vector<Statement *> statements;
    while (!atType(TokenType::ClosedBrace) && !atType(TokenType::Eof)) {
        statements.push_back(declaration());
    }

    if(match({TokenType::ClosedBrace})){
        return statements;
    }

    throw ExpectedXBeforeY(L"}", previous(), at());
}

Statement* Parser::ifStatement(){
    if(!atType(TokenType::OpenParenthesis)){
        throw ExpectedXBeforeY(L"(", previous(), at());
    }
    advance();
    ExprPtr condition = expression();
    if(!atType(TokenType::ClosedParenthesis)){
        throw ExpectedXBeforeY(L")", previous(), at());
    }
    advance();

    StmtPtr thenBranch = statement();
    StmtPtr elseBranch = nullptr;

    if(match({TokenType::Else})){
        elseBranch = statement();
    }
    return new IfStatement(condition, thenBranch, elseBranch);
}

Statement* Parser::expressionStatement() {
    ExprPtr value = expression();
    if(match({TokenType::Semicolon})){
        return new ExpressionStatement(value);
    }
    throw ExpectedXBeforeY(L";", previous(), at());
}

ExprPtr Parser::expression() {
    return assignmentExpression();
}

ExprPtr Parser::assignmentExpression() {
    ExprPtr expr = logicalOrExpression();
    if(match({TokenType::Equal})){
        ExprPtr value = assignmentExpression();
        if(expr->type == AstNodeType::VariableExpression){
            auto* var = static_cast<VariableExpression*>(expr);
            return new AssignmentExpression(var->name, value);
        }
        throw InvalidLValue(getMostRelevantToken(expr));
    }
    return expr;
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
    while(match({TokenType::Slash, TokenType::Star, TokenType::Percent})){
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
    if(match({TokenType::Identifier})) return new VariableExpression(previous());
    if(match({TokenType::OpenParenthesis})){
        ExprPtr expr = expression();
        if(match({TokenType::ClosedParenthesis})){
            return new GroupingExpression(expr);
        }
        throw ExpectedXBeforeY(L"zatvorena zagrada", previous(), at());
    }
    if(current == 0){
        throw ParserError(ERROR_EXPECTED_EXPRESSION_AT_START, at());
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
