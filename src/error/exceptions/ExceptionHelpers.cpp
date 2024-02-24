//
// Created by kemal on 2/24/2024.
//

#include "ExceptionHelpers.h"
#include "BinaryExpression.h"
#include "UnaryExpression.h"
#include "NumericLiteralExpression.h"
#include "BooleanLiteralExpression.h"
#include "StringLiteralExpression.h"
#include "NullLiteralExpression.h"

Token* getMostRelevantToken(const Expression* expression) {
    if (expression->type == AstNodeType::BinaryExpression) {
        auto binaryExpression = static_cast<const BinaryExpression*>(expression);
        return binaryExpression->op;
    }
    if (expression->type == AstNodeType::UnaryExpression) {
        auto unaryExpression = dynamic_cast<const UnaryExpression*>(expression);
        return getMostRelevantToken(unaryExpression->expr);
    }

    if (expression->type == AstNodeType::NumericLiteralExpression) {
        auto numericLiteralExpression = static_cast<const NumericLiteralExpression*>(expression);
        return numericLiteralExpression->token;
    }

    if (expression->type == AstNodeType::BooleanLiteralExpression) {
        auto booleanLiteralExpression = static_cast<const BooleanLiteralExpression*>(expression);
        return booleanLiteralExpression->token;
    }

    if (expression->type == AstNodeType::StringLiteralExpression) {
        auto stringLiteralExpression = static_cast<const StringLiteralExpression*>(expression);
        return stringLiteralExpression->token;
    }

    if (expression->type == AstNodeType::NullLiteralExpression) {
        auto nullLiteralExpression = static_cast<const NullLiteralExpression*>(expression);
        return nullLiteralExpression->token;
    }
    throw std::runtime_error("GET MOST RELEVANT TOKEN: Unknown expression type");
}

