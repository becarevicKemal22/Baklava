//
// Created by kemal on 6/8/2024.
//

#ifndef BAKLAVA_INDEXINGNONARRAY_H
#define BAKLAVA_INDEXINGNONARRAY_H

#include "RuntimeError.h"
#include "ExceptionHelpers.h"
#include "RuntimeValue.h"
#include "Expression.h"


/**
 * @brief Exception thrown when trying to index a non-array value, such as a string or a number.
 */
class IndexingNonArray : public RuntimeError {
public:
    IndexingNonArray(Expression* expression, RuntimeValue value) : RuntimeError(ErrorCode::ERROR_INDEXING_NON_ARRAY) {
        messageArguments.emplace_back(getRuntimeValueTypeString(value));
        this->token = getMostRelevantToken(expression);
    }
    Token* token;
};

#endif //BAKLAVA_INDEXINGNONARRAY_H
