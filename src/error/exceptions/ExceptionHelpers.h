//
// Created by kemal on 2/24/2024.
//

#ifndef BAKLAVA_EXCEPTIONHELPERS_H
#define BAKLAVA_EXCEPTIONHELPERS_H

#include "Token.h"
#include "Expression.h"

/**
 * @brief Get the most relevant token to be highlighted from the given expression.
 *
 * Used by the error printer to know what part of the code to highlight. "Most relevant" means that the function searches a bit deeper than the token of the expression itself. For example, a unary expression node holds a token of the operator, but the most relevant token to highlight is the token of the operand.
 * @param expression The expression to get the most relevant token from
 * @return The most relevant token from the expression
 */
Token* getMostRelevantToken(const Expression* expression);

#endif //BAKLAVA_EXCEPTIONHELPERS_H
