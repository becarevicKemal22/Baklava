//
// Created by kemal on 3/13/2024.
//

#ifndef MATURSKI_2_UNINITIALIZEDCONST_H
#define MATURSKI_2_UNINITIALIZEDCONST_H

#include "ParserError.h"
#include "Token.h"

/**
 * @brief Represents an error where a constant variable was declared without an initializer.
 */
class UninitializedConst : public ParserError {
public:
    UninitializedConst(TokenPtr identifier) : ParserError(ERROR_UNINITIALIZED_CONST), identifier(identifier){
        messageArguments.push_back(identifier->value);
    }

    TokenPtr identifier;
};

#endif //MATURSKI_2_UNINITIALIZEDCONST_H
