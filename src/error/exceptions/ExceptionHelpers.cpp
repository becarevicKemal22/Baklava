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
#include "VariableExpression.h"
#include "CallExpression.h"
#include "LogicalExpression.h"
#include "GroupingExpression.h"
#include "AssignmentExpression.h"
#include "IndexAssignmentExpression.h"
#include "IndexingExpression.h"
#include "ArrayLiteralExpression.h"

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
    if(expression->type == AstNodeType::VariableExpression) {
        auto variableExpression = static_cast<const VariableExpression*>(expression);
        return variableExpression->name;
    }
    if(expression->type == AstNodeType::CallExpression) {
        auto callExpression = static_cast<const CallExpression*>(expression);
        return getMostRelevantToken(callExpression->callee);
    }
    if(expression->type == AstNodeType::LogicalExpression) {
        auto logicalExpression = static_cast<const LogicalExpression*>(expression);
        return logicalExpression->op;
    }
    if(expression->type == AstNodeType::GroupingExpression) {
        auto groupingExpression = static_cast<const GroupingExpression*>(expression);
        return getMostRelevantToken(groupingExpression->expr);
    }
    if(expression->type == AstNodeType::AssignmentExpression) {
        auto assignmentExpression = static_cast<const AssignmentExpression*>(expression);
        return assignmentExpression->name;
    }
    if(expression->type == AstNodeType::IndexingExpression) {
        auto indexingExpression = static_cast<const IndexingExpression*>(expression);
        return getMostRelevantToken(indexingExpression->left);
    }
    if(expression->type == AstNodeType::IndexAssignmentExpression) {
        auto indexAssignmentExpression = static_cast<const IndexAssignmentExpression*>(expression);
        return getMostRelevantToken(indexAssignmentExpression->left);
    }
    if(expression->type == AstNodeType::ArrayLiteralExpression) {
        auto arrayLiteralExpression = static_cast<const ArrayLiteralExpression*>(expression);
        return arrayLiteralExpression->bracket;
    }
    throw std::runtime_error("INTERNAL ERROR IN GET MOST RELEVANT TOKEN: Unknown expression type");
}

