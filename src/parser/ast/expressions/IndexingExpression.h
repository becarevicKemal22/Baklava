//
// Created by kemal on 5/3/2024.
//

#ifndef MATURSKI_2_INDEXINGEXPRESSION_H
#define MATURSKI_2_INDEXINGEXPRESSION_H

#include "Expression.h"
#include "Token.h"
#include <vector>

class IndexingExpression : public Expression {
public:
    IndexingExpression(ExprPtr left, TokenPtr bracket, ExprPtr index) : left(left), bracket(bracket), index(index){
        type = AstNodeType::IndexingExpression;
    }
    ExprPtr left;
    TokenPtr bracket;
    ExprPtr index;

};

#endif //MATURSKI_2_INDEXINGEXPRESSION_H
