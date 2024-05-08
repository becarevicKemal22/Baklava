//
// Created by kemal on 2/20/2024.
//

#ifndef BAKLAVA_RUNTIMEVALUE_H
#define BAKLAVA_RUNTIMEVALUE_H

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

class Interpreter;
typedef struct Object Object;

#define IS_OBJ(value) ((value).type == ValueType::Object)

/**
 * @brief Enum class which defines all types of runtime values in the language, excluding specific object types.
 */
enum class ValueType : uint8_t {
    Number,
    Boolean,
    Null,
    Object,
};

typedef struct RuntimeValue RuntimeValue;

/**
 * @brief Discriminated union representing a runtime value.
 */
struct RuntimeValue{
    ValueType type;
    union {
        bool boolean;
        double number;
        Object* object;
    } as;
};

/**
 * @brief returns the type of a runtime value as a string.
 * @param value RuntimeValue to extract type from.
 * @return std::wstring stringified runtime value type.
 */
std::wstring getRuntimeValueTypeString(const RuntimeValue& value);

#endif //BAKLAVA_RUNTIMEVALUE_H
