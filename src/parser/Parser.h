//
// Created by kemal on 2/4/2024.
//

#ifndef MATURSKI_2_PARSER_H
#define MATURSKI_2_PARSER_H

#include <vector>
#include <memory>

#include "Token.h"
#include "Ast.h"

/**
 * @brief The Parser class is responsible for parsing tokens created by the Lexer into an AST.
 */
class Parser {
public:
    /**
     * @brief Only constructor for the Parser class.
     * @param tokens Tokens created by the Lexer.
     */
    explicit Parser(const std::vector<Token*>& tokens) : tokens(tokens) {}

    std::vector<Token*> tokens;

    /**
     * @brief Parses the tokens into an AST.
     * @return Unique pointer to the root "Program" node.
     * @throws Throws ParserError or derived class if an error occurs during parsing. Expected to be caught by the caller.
     */
    std::unique_ptr<Program> parse();
private:
    unsigned int current = 0;

    void advance() { current++; }
    Token* at() { return tokens[current]; }
    Token* previous() { return tokens[current - 1]; }

    /**
     * @brief Checks if the current token is of any of the given types.
     *
     * If the current token is of any of the given types, the current token is advanced and the function returns true. This means that to access the "current" / matched token, previous() should be used.
     * @param types vector of types to check against.
     * @return True if the current token is of any of the given types, false otherwise.
     */
    bool match(const std::vector<TokenType>& types);

    bool atType(TokenType type){
        return tokens[current]->type == type;
    }

    Statement* statement();
    Statement* declaration();
    Statement* varDeclarationStatement();
    Statement* expressionStatement();
    Statement* printStatement();
    std::vector<Statement*> block();
    Statement* ifStatement();
    Statement* whileStatement();
    Statement* forStatement();

    Expression* expression();
    Expression* assignmentExpression();
    Expression* logicalOrExpression();
    Expression* logicalAndExpression();
    Expression* equalityExpression();
    Expression* comparisonExpression();
    Expression* termExpression();
    Expression* factorExpression();
    Expression* unaryExpression();
    Expression* primaryExpression();
};

#endif //MATURSKI_2_PARSER_H
