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
#include "WhileStatement.h"
#include "CallExpression.h"
#include "FunctionDeclarationStatement.h"
#include "ReturnStatement.h"
#include "IndexingExpression.h"
#include "ArrayLiteralExpression.h"
#include "IndexAssignmentExpression.h"
#include "InvalidDefaultParameterPosition.h"
#include "InvalidDefaultParameterValue.h"

#define IS_LITERAL(x) (x->type == AstNodeType::NumericLiteralExpression || x->type == AstNodeType::StringLiteralExpression || x->type == AstNodeType::BooleanLiteralExpression || x->type == AstNodeType::NullLiteralExpression || x->type == AstNodeType::ArrayLiteralExpression)

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
    if(match({TokenType::While})){
        return whileStatement();
    }
    if(match({TokenType::For})){
        return forStatement();
    }
    if(match({TokenType::Function})){
        return functionDeclarationStatement();
    }
    if(match({TokenType::Return})){
        return returnStatement();
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
    bool hasIs = false;
    TokenPtr isToken = nullptr;
    if(atType(TokenType::Is)){
        advance();
        hasIs = true;
        isToken = previous();
    }

    ExprPtr condition = expression();
    if(!atType(TokenType::Then) && !atType(TokenType::OpenBrace) && hasIs){
        ExprPtr literal;
        try{
            literal = primaryExpression();
        }catch(ExpectedXBeforeY& e){
            if(!atType(TokenType::Then)){
                throw ExpectedXBeforeY(L"onda", previous(), at());
            }
            throw;
        }
        if((literal->type != AstNodeType::BooleanLiteralExpression) && (literal->type != AstNodeType::NullLiteralExpression)){
            throw ExpectedXBeforeY(L"onda", getMostRelevantToken(condition), previous());
        }
        isToken->type = TokenType::DoubleEqual; // Required so that in testing this appears as a equality operator, but I can't create a new token becuase I don't have the position and it doesn't exist anyway. So the is token is used in case an error needs to be printed, and the type is changed for testing, maybe its required somewhere else as well idk.
        condition = new BinaryExpression(condition, isToken, literal);
    }
    if(!atType(TokenType::Then) && !atType(TokenType::OpenBrace)){
        throw ExpectedXBeforeY(L"onda", previous(), at());
    }
    if(atType(TokenType::Then)) advance();

    StmtPtr thenBranch = statement();
    StmtPtr elseBranch = nullptr;

    if(match({TokenType::Else})){
        elseBranch = statement();
    }
    return new IfStatement(condition, thenBranch, elseBranch);
}

Statement* Parser::whileStatement() {
    if(!atType(TokenType::OpenParenthesis)){
        throw ExpectedXBeforeY(L"(", previous(), at());
    }
    advance();
    ExprPtr condition = expression();
    if(!atType(TokenType::ClosedParenthesis)){
        throw ExpectedXBeforeY(L")", previous(), at());
    }
    advance();
    StmtPtr body = statement();
    return new WhileStatement(condition, body);
}

Statement* Parser::forStatement() {
    if(!atType(TokenType::OpenParenthesis)){
        throw ExpectedXBeforeY(L"(", previous(), at());
    }
    advance();
    StmtPtr initializer = nullptr;
    if(match({TokenType::Var})){
        initializer = varDeclarationStatement();
    } else if(match({TokenType::Semicolon})){
        initializer = nullptr;
    } else {
        initializer = expressionStatement();
    }
    ExprPtr condition = nullptr;
    if(!atType(TokenType::Semicolon)){
        condition = expression();
    }
    if(!atType(TokenType::Semicolon)){
        throw ExpectedXBeforeY(L";", previous(), at());
    }
    advance();
    ExprPtr increment = nullptr;
    if(!atType(TokenType::ClosedParenthesis)){
        increment = expression();
    }
    if(!atType(TokenType::ClosedParenthesis)){
        throw ExpectedXBeforeY(L")", previous(), at());
    }
    advance();
    StmtPtr body = statement();

    if(increment != nullptr){
        body = new BlockStatement({body, new ExpressionStatement(increment)});
    }
    if(condition == nullptr){
        condition = new BooleanLiteralExpression(new Token(TokenType::True, L"tačno", 0, 0));
    }
    body = new WhileStatement(condition, body);
    if(initializer != nullptr){
        body = new BlockStatement({initializer, body});
    }

    return body;
}

Statement* Parser::functionDeclarationStatement() {

    if(!atType(TokenType::Identifier)){
        throw ExpectedXBeforeY(L"identifikator", previous(), at());
    }
    advance();
    Token* name = previous();
    if(!atType(TokenType::OpenParenthesis)){
        throw ExpectedXBeforeY(L"(", previous(), at());
    }
    advance();

    std::vector<Token*> parameters;
    std::vector<ExprPtr> defaultParameters;
    bool reachedDefaultValues = false; // so i can check whether I have non-default parameters after default ones
    if(!atType(TokenType::ClosedParenthesis)){
        do {
            if(!atType(TokenType::Identifier)){
                throw ExpectedXBeforeY(L"identifikator", previous(), at());
            }
            advance();

            parameters.push_back(previous());

            if(match({TokenType::Equal})) {
                ExprPtr value = expression();
                if(!IS_LITERAL(value)){
                    throw InvalidDefaultParameterValue(parameters[parameters.size() - 1]);
                }

                defaultParameters.push_back(value);
                reachedDefaultValues = true;
            } else if(reachedDefaultValues){ // in case there's no equal (required param) but there were default values before
                throw InvalidDefaultParameterPosition(parameters[parameters.size() - 2]); // -2 because the last one is the current one, and this error message needs the default argument that was in the disallowed position
            }

        } while(match({TokenType::Comma}));
    }
    if(!atType(TokenType::ClosedParenthesis)){
        throw ExpectedXBeforeY(L")", previous(), at());
    }
    advance();
    if(!atType(TokenType::OpenBrace)){
        throw ExpectedXBeforeY(L"{", previous(), at());
    }
    advance();
    std::vector<Statement*> body = block();
    return new FunctionDeclarationStatement(name, parameters, body, defaultParameters);
}

Statement* Parser::expressionStatement() {
    ExprPtr value = expression();
    if(match({TokenType::Semicolon})){
        return new ExpressionStatement(value);
    }
    throw ExpectedXBeforeY(L";", previous(), at());
}

Statement* Parser::returnStatement() {
    Token* keyword = previous();
    ExprPtr value = nullptr;
    if(!atType(TokenType::Semicolon)){
        value = expression();
    }
    if(match({TokenType::Semicolon})){
        return new ReturnStatement(keyword, value);
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
        } else if(expr->type == AstNodeType::IndexingExpression){
            auto* indexing = static_cast<IndexingExpression*>(expr);
            return new IndexAssignmentExpression(indexing->left, indexing->index, value);
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
    while(match({TokenType::NotEqual, TokenType::DoubleEqual})){
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
    return callExpression();
}
// ovo "call" se odnosi i na poziv funkcije i na indeksiranje, i eventualno kasnije na property access
ExprPtr Parser::callExpression() {
    ExprPtr expr = primaryExpression();
    while(true){
        if(match({TokenType::OpenParenthesis})){
            expr = finishCallExpression(expr);
        } else if(match({TokenType::OpenBracket})){
            auto bracket = previous();
            ExprPtr index = expression();
            if(!atType(TokenType::ClosedBracket)){
                throw ExpectedXBeforeY(L"]", previous(), at());
            }
            advance();
            expr = new IndexingExpression(expr, bracket, index);
        } else {
            break;
        }
    }
    return expr;
}

ExprPtr Parser::finishCallExpression(Expression *callee) {
    std::vector<ExprPtr> arguments;
    if(!atType(TokenType::ClosedParenthesis)){
        do {
            arguments.push_back(expression());
        } while(match({TokenType::Comma}));
    }
    if(!atType(TokenType::ClosedParenthesis)){
        throw ExpectedXBeforeY(L")", previous(), at());
    }
    advance();
    return new CallExpression(callee, previous(), arguments);
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
    if(match({TokenType::OpenBracket})){
        return parseArrayLiteral();
    }
    if(current == 0){
        throw ParserError(ERROR_EXPECTED_EXPRESSION_AT_START, at());
    }
    throw ExpectedXBeforeY(L"izraz", previous(), at());
}

Expression* Parser::parseArrayLiteral() {
    std::vector<ExprPtr> elements;
    auto bracket = previous();
    if(!atType(TokenType::ClosedBracket)){
        do {
            elements.push_back(expression());
        } while(match({TokenType::Comma}));
    }
    if(!atType(TokenType::ClosedBracket)){
        throw ExpectedXBeforeY(L"]", previous(), at());
    }
    advance();
    return new ArrayLiteralExpression(elements, bracket);
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
