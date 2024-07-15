//
// Created by kemal on 6/8/2024.
//

#ifndef BAKLAVA_NONINTEGERINDEX_H
#define BAKLAVA_NONINTEGERINDEX_H

#include "RuntimeError.h"
#include "Expression.h"

/**
 * @brief Represents an error where a non-integer index was used to access an array.
 *
 * Not to be used when the index is not a number, rather it is meant to represent a numeric indexing attempt that is
 * not an integer.
 */
class NonIntegerIndex : public RuntimeError {
public:
    NonIntegerIndex(Expression* index, double indexValue) : RuntimeError(ERROR_NON_INTEGER_INDEX), index(index) {
        messageArguments.push_back(std::to_wstring(indexValue));
    }

    Expression* index;
};

#endif //BAKLAVA_NONINTEGERINDEX_H
