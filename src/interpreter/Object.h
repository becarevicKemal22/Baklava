//
// Created by kemal on 3/6/2024.
//

#ifndef MATURSKI_2_OBJECT_H
#define MATURSKI_2_OBJECT_H

#include <cstdint>
#include <string>

#include "RuntimeValue.h"
#include "FunctionDeclarationStatement.h"

#define IS_STRING_OBJ(value) ((value).as.object->type == ObjectType::OBJECT_STRING)
#define AS_STRING_OBJ(value) ((ObjectString*)value.as.object)
#define GET_STRING_OBJ_VALUE(val) (AS_STRING_OBJ(val)->value)

enum class ObjectType : uint8_t{
  OBJECT_STRING,
  OBJECT_CALLABLE,
  OBJECT_FUNCTION,
};

struct Object {
    ObjectType type;
    Object* next;
};

struct ObjectString {
    Object obj;
    std::wstring value;
};

struct ObjectCallable {
    Object obj;
    size_t arity;
    std::function<RuntimeValue(Interpreter*, const std::vector<RuntimeValue>&)> call;
};

struct ObjectFunction : public ObjectCallable {
    FunctionDeclarationStatement* declaration;
    RuntimeValue myCall(Interpreter* interpreter, const std::vector<RuntimeValue>& arguments);
    ObjectFunction(FunctionDeclarationStatement* declaration) : declaration(declaration){
        obj.type = ObjectType::OBJECT_FUNCTION;
        arity = declaration->parameters.size();
        call = [this](auto && PH1, auto && PH2) { return myCall(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); };
    }
};

#endif //MATURSKI_2_OBJECT_H
