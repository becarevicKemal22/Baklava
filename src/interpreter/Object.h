//
// Created by kemal on 3/6/2024.
//

#ifndef BAKLAVA_OBJECT_H
#define BAKLAVA_OBJECT_H

#include <cstdint>
#include <string>

#include "RuntimeValue.h"
#include "FunctionDeclarationStatement.h"
#include "Environment.h"

#define IS_STRING_OBJ(value) ((value).as.object->type == ObjectType::OBJECT_STRING)
#define AS_STRING_OBJ(value) ((ObjectString*)value.as.object)
#define GET_STRING_OBJ_VALUE(val) (AS_STRING_OBJ(val)->value)

#define IS_CALLABLE_OBJ(value) ((value).as.object->type == ObjectType::OBJECT_CALLABLE)
#define AS_CALLABLE_OBJ(value) ((ObjectCallable*)value.as.object)

#define IS_FUNCTION_OBJ(value) ((value).as.object->type == ObjectType::OBJECT_FUNCTION)
#define AS_FUNCTION_OBJ(value) ((ObjectFunction*)value.as.object)

#define IS_ARRAY_OBJ(value) ((value).as.object->type == ObjectType::OBJECT_ARRAY)
#define AS_ARRAY_OBJ(value) ((ObjectArray*)value.as.object)
#define GET_ARRAY_OBJ_ELEMENTS(val) (AS_ARRAY_OBJ(val)->elements)

enum class ObjectType : uint8_t{
  OBJECT_STRING,
  OBJECT_CALLABLE,
  OBJECT_FUNCTION,
  OBJECT_ARRAY
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

struct ObjectArray {
    Object obj;
    std::vector<RuntimeValue> elements;
};

class Environment;

struct ObjectFunction : public ObjectCallable {
    FunctionDeclarationStatement* declaration;
    Environment closure;
    RuntimeValue functionCall(Interpreter* interpreter, const std::vector<RuntimeValue>& arguments);
    // Constructor, too scared to move it to the top of the struct
    explicit ObjectFunction(FunctionDeclarationStatement* declaration, Environment* closure) : declaration(declaration){
        obj.type = ObjectType::OBJECT_FUNCTION;
        arity = declaration->parameters.size();
        this->closure = *closure;
        call = [this](auto && PH1, auto && PH2) { return functionCall(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); };
    }
};

#endif //BAKLAVA_OBJECT_H
