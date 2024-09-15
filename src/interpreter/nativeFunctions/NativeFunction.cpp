//
// Created by kemal on 9/16/2024.
//

#include "NativeFunction.h"

Token* generateIdentifierToken(const std::wstring& name) {
    return new Token(TokenType::Identifier, name, 0, 0);
}