//
// Created by kemal on 3/6/2024.
//

#include <sstream>
#include "Object.h"
#include "Interpreter.h"
#include "Environment.h"

RuntimeValue ObjectFunction::functionCall(Interpreter* interpreter, const std::vector<RuntimeValue>& arguments){
    Environment environment(&closure);
    for (int i = 0; i < declaration->parameters.size(); i++) {
        environment.define(declaration->parameters[i], arguments[i], false);
    }
    interpreter->executeBlock(declaration->body, environment);
    if(interpreter->isReturning){
        interpreter->isReturning = false;
        return interpreter->returnedValue;
    }
    return {ValueType::Null, {}};
}

std::wstring getObjectTypeName(ObjectType type){
    switch (type) {
        case ObjectType::OBJECT_STRING:
            return L"string";
        case ObjectType::OBJECT_CALLABLE:
            return L"callable";
        case ObjectType::OBJECT_FUNCTION:
            return L"function";
        case ObjectType::OBJECT_ARRAY:
            return L"array";
    }
    return L"unknown";
}

std::wstring getObjectIdentifier(Object* object){
    switch (object->type) {
        case ObjectType::OBJECT_STRING:
            return ((ObjectString*)object)->value;
        case ObjectType::OBJECT_CALLABLE:
            return L"";
        case ObjectType::OBJECT_FUNCTION:
            return ((ObjectFunction*)object)->declaration->name->value;
        case ObjectType::OBJECT_ARRAY:{
            ObjectArray* array = (ObjectArray*)object;
            std::wstringstream identifier;
            identifier << L"[";
            size_t size = array->elements.size();
            if(size > 3) size = 3; // limit output to first 3 elements
            for (int i = 0; i < size; i++) {
                if(array->elements[i].type != ValueType::Object){
                    Interpreter::printValue(array->elements[i], identifier);
                } else {
                    identifier << getObjectIdentifier(array->elements[i].as.object);
                }
                if(i != size - 1) identifier << L", ";
            }
            if(array->elements.size() > 3) identifier << L"...";
            identifier << L"]";
            return identifier.str();
        }
    }
    return L"unknown";
}