//
// Created by kemal on 3/13/2024.
//

#include <memory>
#include "Environment.h"
#include "RuntimeValue.h"

void Environment::define(Token* name, RuntimeValue value, bool isConstant) {
    auto it = variables.find(name->value);
    if(it != variables.end()){
        throw "";
    }

    variables.insert({name->value, {value, isConstant}});
}

RuntimeValue Environment::get(Token* name){
    auto it = variables.find(name->value);
    if(it != variables.end()) {
        return it->second.first;
    }
    else if(parent != nullptr) {
        return parent->get(name);
    }
    throw "";
}

void Environment::assign(Token* name, RuntimeValue value) {
    auto it = variables.find(name->value);
    if(it == variables.end()){
        if(parent != nullptr){
            parent->assign(name, value);
            return;
        }
        throw "";
    }
    if(it->second.second){
        throw "";
    }
    variables[name->value] = {value, false};
}
