//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_TESTHELPERS_H
#define MATURSKI_2_TESTHELPERS_H

#include <catch2/catch_test_macros.hpp>

#include "Statement.h"

template<typename T>
T* getNode(Statement* node) {
    T* castedNode = dynamic_cast<T*>(node);
    REQUIRE(castedNode != nullptr);
    return castedNode;
}

#endif //MATURSKI_2_TESTHELPERS_H
