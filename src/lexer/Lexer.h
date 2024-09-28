//
// Created by kemal on 1/28/2024.
//

#ifndef BAKLAVA_LEXER_H
#define BAKLAVA_LEXER_H

#include <string>
#include <vector>

#include "Token.h"
#include "ErrorPrinter.h"

/**
 * @brief Class that is used to tokenize source code into tokens.
 *
 * The tokens that this class produces are used not only by the parser, but are pointed to by AST nodes and are used in the interpreting stage too, so it is important that they are not deleted before the interpreter is finished or quits, or that the ownership of them is transferred to the AST nodes or the interpreter.
 */
class Lexer {
public:
    /**
     * @brief Construct a new Lexer instance
     *
     * This constructor is mostly made for testing purposes, when error printing is not needed.
     * @param source Source code to tokenize.
     */
    Lexer(const std::wstring&  source) : source(source), printer(nullptr) {}

    /**
     * @brief Construct a new Lexer instance with an error printer.
     *
     * This is the preferred constructor to use, as it allows for error reporting.
     * @param source reference to wstring source code to tokenize.
     * @param printer ErrorPrinter pointer to be used for error printing.
     */
    Lexer(const std::wstring& source, ErrorPrinter* printer) : source(source), printer(printer) {}

    /**
     * @brief Destroy the Lexer instance
     *
     * This destructor should currently not delete tokens, as that can crash the program if the lexer is not in the same environment as the interpreter.
     */
    ~Lexer(){

    };

    std::vector<Token*> tokens{}; /**< Vector of token pointers that are dynamically allocated as the tokenize function is run. The tokens in this vector are being pointed to during the whole lifetime of the program, so it is important that they are not deleted before the interpreter is finished or quits. */

    /**
     * @brief Tokenizes the source code.
     *
     * This function tokenizes the source code and populates the tokens vector with tokens.
     * @throws std::runtime_error throws if an invalid character is found in the source code and prints error if printer is not nullptr.
     */
    void tokenize();

    /**
     * @brief Sets the error printer.
     *
     * @param printer ErrorPrinter pointer to be used for error printing.
     */
    void setPrinter(ErrorPrinter* printer){
        this->printer = printer;
    }
private:
    std::wstring source;
    unsigned int currentChar = 0;
    unsigned int line = 1;
    unsigned int charIndexOnLine = 0;
    ErrorPrinter* printer;

    std::vector<Token*> keywordHistory; /**< Holds found keyword tokens because keywords can not be immediately pushed into the tokens vector, in case they are part of a keyword combination. Tokens will only be pushed into here until a non-keyword token is found, after which they will be concatenated if needed and pushed along with the latest non-keyword token. */

    /**
     * @brief Dynamically allocates a single character token and pushes it into the tokens vector.
     *
     * @param type
     * @param character
     */
    void addToken(TokenType type, wchar_t character);

    /**
     * @brief Dynamically allocates a multi-character token and pushes it into the tokens vector.
     *
     * @param type
     * @param value
     * @param offsetIsStartOfToken If true, the offset of the token will be the same as the current offset. If false, the offset will be the current offset minus the length of the value.
     */
    void addToken(TokenType type, const std::wstring& value, bool offsetIsStartOfToken = false);

    /**
     * @brief Dynamically allocates a token and returns it. Technically it is unecessary, but because tokens are also pushed
     * into the keywordHistory vector, I wanted a standardized method that can be used both there and in addToken.
     *
     * @param type
     * @param value
     * @param offsetIsStartOfToken If true, the offset of the token will be the same as the current offset. If false, the offset will be the current offset minus the length of the value.
     * @return TokenPtr to the newly allocated token.
     */
    TokenPtr generateToken(TokenType type, const std::wstring& value, bool offsetIsStartOfToken = false);

    /**
     * @brief Dynamically allocates a string token and pushes it into the tokens vector.
     *
     * @param value
     * @param line
     * @param offset the offset of the " character in the source code which starts the string.
     */
    void addStringToken(const std::wstring& value, unsigned int line, unsigned int offset);

    void advance() { currentChar++; charIndexOnLine++;}
    wchar_t peek() { return source[currentChar + 1];}
    void handleTab();

    /**
     * @brief Handles newlines character.
     *
     * It also handles the case of CR+LF sequence. If there is issues with line numbering it is most likely here.
     */
    void handleNewLine();

    /**
     * @brief Handles string literals.
     *
     * @param startQuote The quote that starts the string. Can be either a double quote or a single quote. Required so that a string cannot start and end with a different symbol.
     */
    void handleString(wchar_t startQuote);
    void handleComment();

    /**
     * @brief Handles keyword combinations such as "manje ili jednako od".
     *
     * Only executes after a non-keyword token is found, after which it creates new tokens for any combinations that are found
     * and pushes them into the tokens vector. At the end re-pushes the non-keyword token that was found and clears the keywordHistory vector.
     */
    void handleKeywordCombinations();

    /**
     * @brief Checks if a token is a keyword.
     *
     * @param token TokenPtr pointer to the token to check.
     * @return true if the token is a keyword, false otherwise.
     */
    bool isKeyword(TokenPtr token);

    /**
     * @brief Matches passed vector to the combinations found in KEYWORD_COMBINATIONS.
     * @param types Vector of TokenType to match.
     * @return true if the vector matches a combination in KEYWORD_COMBINATIONS, false otherwise.
     */
    bool isCombination(const std::vector<TokenType>& types);

    /**
     * @brief Combines a vector of consecutive tokens into one token. It's important that they are consecutive, as
     * the function puts whitespace between the tokens. (amount of spaces is equal to the difference of end offset of the first token and start offset of the second token)
     *
     * @param tokens Vector of TokenPtr to combine.
     * @param newType TokenType of the new token.
     * @return TokenPtr to the combined token.
     */
    TokenPtr combineConsecutiveTokens(const std::vector<TokenPtr>& tokens, TokenType newType);
};


#endif //BAKLAVA_LEXER_H
