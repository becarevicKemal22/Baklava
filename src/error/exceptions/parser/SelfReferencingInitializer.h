//
// Created by kemal on 3/27/2024.
//

#ifndef MATURSKI_2_SELFREFERENCINGINITIALIZER_H
#define MATURSKI_2_SELFREFERENCINGINITIALIZER_H

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

#endif //MATURSKI_2_SELFREFERENCINGINITIALIZER_H
