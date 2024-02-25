//
// Created by kemal on 2/25/2024.
//

#ifndef MATURSKI_2_WRONGBINARYOPERANDTYPES_H
#define MATURSKI_2_WRONGBINARYOPERANDTYPES_H

#include "RuntimeError.h"
#include "RuntimeValue.h"
#include "Expression.h"
#include "ExceptionHelpers.h"
#include "BinaryExpression.h"

class WrongBinaryOperandTypes : public RuntimeError {
public:
    WrongBinaryOperandTypes(const std::wstring& toWhat, const RuntimeValue& left, const RuntimeValue& right, const Expression* expression) : RuntimeError(ErrorCode::ERROR_WRONG_TYPE_OPERAND_BINARY) {
        messageArguments.emplace_back(valueTypeToString(left.type));
        messageArguments.emplace_back(valueTypeToString(right.type));
        messageArguments.emplace_back(toWhat);
        this->token = getMostRelevantToken(expression);
        auto binaryExpr = dynamic_cast<const BinaryExpression*>(expression);
        if(binaryExpr != nullptr) {
            this->leftToken = getMostRelevantToken(binaryExpr->left);
            this->rightToken = getMostRelevantToken(binaryExpr->right);
        }else{
            throw "INTERNAL EXCEPTION ERROR: Invalid expression type. Expected BinaryExpression.";
        }
    }
    Token* token;
    Token* leftToken;
    Token* rightToken;
};

#endif //MATURSKI_2_WRONGBINARYOPERANDTYPES_H
