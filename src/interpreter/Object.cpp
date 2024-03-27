//
// Created by kemal on 3/6/2024.
//

#include "Object.h"
#include "Interpreter.h"
#include "Environment.h"

RuntimeValue ObjectFunction::functionCall(Interpreter* interpreter, const std::vector<RuntimeValue>& arguments){
    auto environment = new Environment(&closure);
    for (int i = 0; i < declaration->parameters.size(); i++) {
        environment->define(declaration->parameters[i], arguments[i], false);
    }
    interpreter->executeBlock(declaration->body, environment);
    if(interpreter->isReturning){
        interpreter->isReturning = false;
        return interpreter->returnedValue;
    }
    return {ValueType::Null, {}};
}