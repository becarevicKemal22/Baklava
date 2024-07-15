//
// Created by kemal on 2/24/2024.
//

#include "RuntimeValue.h"
#include "Object.h"

std::wstring getRuntimeValueTypeString(const RuntimeValue &value) {
    switch (value.type) {
        case ValueType::Number:
            return L"broj";
        case ValueType::Boolean:
            return L"boole";
        case ValueType::Null:
            return L"null";
        case ValueType::Object:
            switch (value.as.object->type) {
                case ObjectType::OBJECT_STRING:
                    return L"string";
                case ObjectType::OBJECT_ARRAY:
                    return L"niz";
                case ObjectType::OBJECT_FUNCTION:
                case ObjectType::OBJECT_CALLABLE:
                    return L"funkcija";
                default:
                    return L"nepoznat objekat";
            }
        default:
            return L"nepoznat tip";
    }
}