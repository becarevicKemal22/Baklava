//
// Created by kemal on 3/13/2024.
//

#ifndef BAKLAVA_UNINITIALIZEDCONST_H
#define BAKLAVA_UNINITIALIZEDCONST_H

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

#endif //BAKLAVA_UNINITIALIZEDCONST_H
