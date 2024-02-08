//
// Created by kemal on 11/11/2023.
//

#include "ValueType.h"
#include <string>

std::string stringifyRuntimeType(ValueType type) {
    switch (type) {
        case ValueType::Number:
            return "Broj";
        case ValueType::Boolean:
            return "Boole";
        case ValueType::String:
            return "Rijec";
        case ValueType::Null:
            return "Prazno";
        case ValueType::Callable:
            return "Callable";
        case ValueType::Function:
            return "Funkcija";
    }
}