//
// Created by kemal on 7/17/2024.
//

#include <catch2/catch_test_macros.hpp>

#include "RuntimeValue.h"
#include "Object.h"

TEST_CASE("Correctly stringifies runtime value types", "[runtimeValue]") {
    RuntimeValue number(ValueType::Number);
    REQUIRE(getRuntimeValueTypeString(number) == L"broj");

    RuntimeValue boole(ValueType::Boolean);
    REQUIRE(getRuntimeValueTypeString(boole) == L"boole");

    RuntimeValue null(ValueType::Null);
    REQUIRE(getRuntimeValueTypeString(null) == L"null");

    Object str(ObjectType::OBJECT_STRING);
    RuntimeValue string(ValueType::Object);
    string.as.object = &str;
    REQUIRE(getRuntimeValueTypeString(string) == L"string");

    Object arr(ObjectType::OBJECT_ARRAY);
    RuntimeValue array(ValueType::Object);
    array.as.object = &arr;
    REQUIRE(getRuntimeValueTypeString(array) == L"niz");

    Object func(ObjectType::OBJECT_FUNCTION);
    RuntimeValue function(ValueType::Object);
    function.as.object = &func;
    REQUIRE(getRuntimeValueTypeString(function) == L"funkcija");

    Object callable(ObjectType::OBJECT_CALLABLE);
    RuntimeValue callableValue(ValueType::Object);
    callableValue.as.object = &callable;
    REQUIRE(getRuntimeValueTypeString(callableValue) == L"funkcija");
}