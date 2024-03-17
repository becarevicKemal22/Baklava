//
// Created by kemal on 3/13/2024.
//

#include "Environment.h"
#include "RuntimeValue.h"
#include "VariableRedeclaration.h"
#include "UndeclaredVariable.h"
#include "ConstReassignment.h"

void Environment::define(Token* name, RuntimeValue value, bool isConstant) {
    auto it = variables.find(name->value);
    if(it != variables.end()){
        throw VariableRedeclaration(name);
    }

    variables.insert({name->value, {value, isConstant}});
}

RuntimeValue Environment::get(Token* name){
    auto it = variables.find(name->value);
    if(it != variables.end()) {
        return it->second.first;
    }
    if(parent != nullptr) {
        return parent->get(name);
    }
    throw UndeclaredVariable(name);
}

void Environment::assign(Token* name, RuntimeValue value) {
    auto it = variables.find(name->value);
    if(it == variables.end()){
        if(parent != nullptr){
            parent->assign(name, value);
            return;
        }
        throw UndeclaredVariable(name);
    }
    if(it->second.second){
        throw ConstReassignment(name);
    }
    variables[name->value] = {value, false};
}

RuntimeValue Environment::getAt(int distance, const std::wstring& name){
    return ancestor(distance)->variables[name].first;
}

Environment* Environment::ancestor(int distance) {
    Environment *environment = this;
    for (int i = 0; i < distance; i++) {
        environment = environment->parent;
    }
    return environment;
}

void Environment::assignAt(int distance, const std::wstring& name, RuntimeValue value){
    ancestor(distance)->variables[name].first = std::move(value);
}