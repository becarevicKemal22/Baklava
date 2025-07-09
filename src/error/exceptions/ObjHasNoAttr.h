//
// Created by kemal on 05-Jul-25.
//

#ifndef OBJHASNOATTR_H
#define OBJHASNOATTR_H

#include "RuntimeError.h"
#include "RuntimeValue.h"
#include "Token.h"

/**
 * @brief Represents an error where a non-existent property was accessed on an object. This does not apply to property access on non-instances, that is InvalidPropertyAccess.
 */
class ObjHasNoAttr : public RuntimeError {
public:
    ObjHasNoAttr(TokenPtr property, const RuntimeValue& val) : RuntimeError(ERROR_OBJ_HAS_NO_ATTR), token(property) {
        messageArguments.push_back(getRuntimeValueTypeString(val));
        messageArguments.push_back(property->value);
    }
    TokenPtr token;
};

#endif //OBJHASNOATTR_H
