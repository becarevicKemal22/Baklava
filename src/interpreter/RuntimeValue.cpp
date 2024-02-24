//
// Created by kemal on 2/24/2024.
//

#include "RuntimeValue.h"

std::wstring valueTypeToString(ValueType type) {
    switch (type) {
        case ValueType::Number:
            return L"broj";
        case ValueType::Boolean:
            return L"boole";
        case ValueType::Null:
            return L"nula";
    }
}