//
// Created by kemal on 3/13/2024.
//

#ifndef MATURSKI_2_ENVIRONMENT_H
#define MATURSKI_2_ENVIRONMENT_H

#include <unordered_map>
#include <algorithm>
#include "RuntimeValue.h"
#include "Token.h"

class Environment {
public:
    Environment() : parent(nullptr) {};
    explicit Environment(Environment* parent) : parent(parent) {}
    std::unordered_map<std::wstring, std::pair<RuntimeValue, bool>> variables{}; // Holds a map of variable names bound to a std::pair that holds the runtime value and whether the variable is constant
    Environment* parent; /** Points to the parent environment of the env. If this is nullptr, it means that the environment is the global one. */

    /**
     * Makes a new variable in the environment
     * @param name Token pointer that points to the token of the variable identifier
     * @param value RuntimeValue* that can either hold a value or be nullptr. In case it is nullptr, variable will be initialized to null. Important to note is that it does not check if a const variable has tried to be initialized without a value. That is something that must be checked beforehand.
     * @param isConstant bool value that defines whether the variable should be a constant or not.
     * @throws VariableRedeclaration if name parameter already matches an existing value in the environment.
     */
    void define(Token* name, RuntimeValue value, bool isConstant);
    /**
     *
     * @param name String variable that provides the identifier of the desired variable
     * @return RuntimeValue if it has been found in the environment or some parent environment
     * @throws UndeclaredVariable Throws if provided an identifier of a non-existing variable
     */
    RuntimeValue get(Token* name);

    /**
     * Assigns a value to an existing variable
     * @param name Token pointer that holds the identifier of the variable
     * @param value RuntimeValue that holds the value to be used for assignment
     * @throws UndeclaredVariable Throws if provided an identifier of a non-existing variable
     * @throws ConstReassignment Throws if assign is called on a constant variable
     */
    void assign(Token* name, RuntimeValue value);

    RuntimeValue getAt(int distance, const std::wstring& name){
        return ancestor(distance)->variables[name].first;
    }

    Environment* ancestor(int distance) {
        Environment *environment = this;
        for (int i = 0; i < distance; i++) {
            environment = environment->parent;
        }
        return environment;
    }
    void assignAt(int distance, const std::wstring& name, RuntimeValue value){
        ancestor(distance)->variables[name].first = std::move(value);
    }
};


#endif //MATURSKI_2_ENVIRONMENT_H
