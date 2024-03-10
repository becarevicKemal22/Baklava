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

/**
 * @brief Exception thrown when a binary operator is used with two operands of the wrong type. Mainly used for binary operators.
 *
 * Also gets the most relevant tokens from the given incompatible expressions
 */
class WrongBinaryOperandTypes : public RuntimeError {
public:
    WrongBinaryOperandTypes(const std::wstring &toWhat, const RuntimeValue &left, const RuntimeValue &right,
                            const BinaryExpression *expression) : RuntimeError(
            ErrorCode::ERROR_WRONG_TYPE_OPERAND_BINARY) {
        messageArguments.emplace_back(getRuntimeValueTypeString(left));
        messageArguments.emplace_back(getRuntimeValueTypeString(right));
        messageArguments.emplace_back(toWhat);
        this->token = getMostRelevantToken(expression);

        this->leftToken = getMostRelevantToken(expression->left);
        this->rightToken = getMostRelevantToken(expression->right);

    }

    Token *token;
    Token *leftToken;
    Token *rightToken;
};

#endif //MATURSKI_2_WRONGBINARYOPERANDTYPES_H
