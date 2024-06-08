//
// Created by kemal on 6/7/2024.
//

#ifndef BAKLAVA_INDEXOUTOFBOUNDS_H
#define BAKLAVA_INDEXOUTOFBOUNDS_H

#include "RuntimeError.h"

class IndexOutOfBounds : public RuntimeError {
public:
    IndexOutOfBounds(Expression* index, int indexValue) : RuntimeError(ERROR_INDEX_OUT_OF_BOUNDS), index(index) {
        messageArguments.push_back(std::to_wstring(indexValue));
    }

    Expression* index;
};

#endif //BAKLAVA_INDEXOUTOFBOUNDS_H
