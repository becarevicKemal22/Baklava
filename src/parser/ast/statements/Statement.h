//
// Created by kemal on 2/5/2024.
//

#ifndef MATURSKI_2_STATEMENT_H
#define MATURSKI_2_STATEMENT_H

#include "Ast.h"

/**
 * @brief Base class for all AST nodes. Not meant to be used aside from achieving polymorphism.
 *
 * All AST nodes must inherit from this class.
 */
class Statement {
public:
    Statement() = default;
    virtual ~Statement() = default;
    AstNodeType type;
};

#endif //MATURSKI_2_STATEMENT_H
