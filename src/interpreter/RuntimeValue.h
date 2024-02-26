//
// Created by kemal on 2/20/2024.
//

#ifndef MATURSKI_2_RUNTIMEVALUE_H
#define MATURSKI_2_RUNTIMEVALUE_H

#include <cstdint>
#include <string>

/**
 * @brief Enum class which defines all types of runtime values in the language.
 */
enum class ValueType : uint8_t {
    Number,
    Boolean,
    Null,
};

/**
 * @brief Discriminated union representing a runtime value.
 */
typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} RuntimeValue;

/**
 * @brief Converts a ValueType to a string.
 * @param type ValueType to convert.
 * @return std::wstring representation of the given ValueType.
 */
std::wstring valueTypeToString(ValueType type);

#endif //MATURSKI_2_RUNTIMEVALUE_H
