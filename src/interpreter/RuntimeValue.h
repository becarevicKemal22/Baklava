//
// Created by kemal on 2/20/2024.
//

#ifndef MATURSKI_2_RUNTIMEVALUE_H
#define MATURSKI_2_RUNTIMEVALUE_H

#include <cstdint>
#include <string>

#include "Object.h"

/**
 * @brief Enum class which defines all types of runtime values in the language.
 */
enum class ValueType : uint8_t {
    Number,
    Boolean,
    Null,
    Object,
};

/**
 * @brief Discriminated union representing a runtime value.
 */
typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
        Object* object;
    } as;
} RuntimeValue;

/**
 * @brief returns the type of a runtime value as a string.
 * @param value RuntimeValue to extract type from.
 * @return std::wstring stringified runtime value type.
 */
std::wstring getRuntimeValueTypeString(const RuntimeValue& value);

#endif //MATURSKI_2_RUNTIMEVALUE_H
