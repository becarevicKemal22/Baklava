//
// Created by kemal on 3/13/2024.
//

#ifndef BAKLAVA_ENVIRONMENT_H
#define BAKLAVA_ENVIRONMENT_H

#include <unordered_map>
#include <algorithm>
#include "Token.h"

#include <iostream>

typedef struct RuntimeValue RuntimeValue;

class Environment {
public:
    Environment() : parent(nullptr) {};
    explicit Environment(Environment* parent) : parent(parent) {}

    std::unordered_map<std::wstring, std::pair<RuntimeValue, bool>> variables{}; // Holds a map of variable names bound to a std::pair that holds the runtime value and whether the variable is constant
    Environment* parent; /* Points to the parent environment of the env. If this is nullptr, it means that the environment is the global one. */

    /**
     * Makes a new variable in the environment
     * @param name Token pointer that points to the token of the variable identifier
     * @param value RuntimeValue* that can either hold a value or be nullptr. In case it is nullptr, variable will be initialized to null. Important to note is that it does not check if a const variable has tried to be initialized without a value. That is something that must be checked beforehand.
     * @param isConstant bool value that defines whether the variable should be a constant or not.
     * @throws IdentifierRedeclaration if name parameter already matches an existing value in the environment.
     */
    void define(Token* name, RuntimeValue value, bool isConstant);

    /**
     * SHOULD NOT BE USED WITHOUT A VERY GOOD REASON. CURRENTLY ONLY USED IN DEFINEANDBINDTHIS.
     * Makes a new variable in the environment without requiring a token. Used for special cases such as 'this' binding
     * for method calls. Should not be used if there is not a very good reason. Error reporting is not going to work
     * properly since a mock token will be created in the case of an exception. A good rule is to only use this
     * method if it can be guaranteed that calling it with the given name can not cause a IdentifierRedeclaration error.
     * @param name  wstring that contains the name of the variable identifier
     * @param value RuntimeValue* that can either hold a value or be nullptr. In case it is nullptr, variable will be initialized to null. Important to note is that it does not check if a const variable has tried to be initialized without a value. That is something that must be checked beforehand.
     * @param isConstant bool value that defines whether the variable should be a constant or not.
     * @throws IdentifierRedeclaration if name parameter already matches an existing value in the environment.
     */
    void defineByNameString(const std::wstring& name, RuntimeValue value, bool isConstant);

    /**
     * Defines all variants of the 'this' keyword in the environment and sets them to be constant and to reference
     * the instance passed in as the parameter. Does not do any checks; it simply defines the 'this' keywords using
     * defineByNameString. Mainly implemented to have the different versions of 'this' at a single place (at least for the
     * runtime part of the programming language).
     * @param instance The instance that 'this' should be bound to.
     */
    void defineAndBindThis(RuntimeValue instance);

    /**
     *
     * @param name String variable that provides the identifier of the desired variable
     * @return RuntimeValue if it has been found in the environment or some parent environment
     * @throws UndeclaredIdentifier Throws if provided an identifier of a non-existing variable
     */
    RuntimeValue get(Token* name);

    /**
     * Assigns a value to an existing variable
     * @param name Token pointer that holds the identifier of the variable
     * @param value RuntimeValue that holds the value to be used for assignment
     * @throws UndeclaredIdentifier Throws if provided an identifier of a non-existing variable
     * @throws ConstReassignment Throws if assign is called on a constant variable
     */
    void assign(Token* name, RuntimeValue value);

    RuntimeValue getAt(int distance, const std::wstring& name);

    Environment* ancestor(int distance);

    void assignAt(int distance, const std::wstring& name, RuntimeValue value);
};


#endif //BAKLAVA_ENVIRONMENT_H
