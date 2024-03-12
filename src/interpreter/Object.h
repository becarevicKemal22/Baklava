//
// Created by kemal on 3/6/2024.
//

#ifndef MATURSKI_2_OBJECT_H
#define MATURSKI_2_OBJECT_H

#include <cstdint>
#include <string>

#define IS_STRING_OBJ(value) ((value).as.object->type == ObjectType::OBJECT_STRING)
#define AS_STRING_OBJ(value) ((ObjectString*)value.as.object)
#define GET_STRING_OBJ_VALUE(val) (AS_STRING_OBJ(val)->value)

enum class ObjectType : uint8_t{
  OBJECT_STRING,
};

struct Object {
    ObjectType type;
    Object* next;
};

struct ObjectString {
    Object obj;
    std::wstring value;
};


#endif //MATURSKI_2_OBJECT_H
