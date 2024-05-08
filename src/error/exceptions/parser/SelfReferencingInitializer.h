//
// Created by kemal on 3/27/2024.
//

#ifndef BAKLAVA_SELFREFERENCINGINITIALIZER_H
#define BAKLAVA_SELFREFERENCINGINITIALIZER_H

#include "ParserError.h"
#include "Token.h"

/**
 * @brief Represents an error where a local variable was initialized using itself.
 */
class SelfReferencingInitializer : public ParserError {
public:
    SelfReferencingInitializer(TokenPtr token) : ParserError(ERROR_SELF_REFERENCING_INITIALIZER), token(token) {
        messageArguments.push_back(token->value);
    }

    TokenPtr token;
};

#endif //BAKLAVA_SELFREFERENCINGINITIALIZER_H
