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
    bool isMarked {false};
};

struct ObjectString {
    Object obj;
    std::wstring value;
};

struct ObjectCallable {
    Object obj;
    size_t arity;
    size_t minArity;
    std::vector<RuntimeValue> defaultArguments; // refers to optional arguments and their values
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
        minArity = arity - declaration->defaultParameters.size();
        this->closure = *closure;
        call = [this](auto && PH1, auto && PH2) { return functionCall(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); };
    }
};

/**
 * @brief Stringifies the object type name.
 *
 * @param type The type of the object to stringify.
 * @return String value of the name of the object type.
 */
std::wstring getObjectTypeName(ObjectType type);

/**
 * @brief Gets the identifier of the object.
 *
 * It is a string that can be used to identify the object when printing or logging. It is not guaranteed to be unique. Can be a name, value, or anything else appropriate for the passed object type.
 *
 * @param object The object to get the identifier of.
 * @return String representing the identifier of the object.
 */
std::wstring getObjectIdentifier(Object* object);

#endif //BAKLAVA_OBJECT_H
