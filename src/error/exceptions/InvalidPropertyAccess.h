//
// Created by kemal on 05-Jul-25.
//

#ifndef INVALIDPROPERTYACCESS_H
#define INVALIDPROPERTYACCESS_H

#include "RuntimeError.h"
#include "RuntimeValue.h"

/**
 * @brief Represents an error where a property access was done on a non-instance
 */
class InvalidPropertyAccess : public RuntimeError {
public:
    InvalidPropertyAccess(TokenPtr property, const RuntimeValue& val) : RuntimeError(ERROR_INVALID_PROPERTY_ACCESS), token(property) {
        messageArguments.push_back(property->value);
        messageArguments.push_back(getRuntimeValueTypeString(val));
    }
    TokenPtr token;
};

#endif //INVALIDPROPERTYACCESS_H
