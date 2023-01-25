#pragma once
#include "IStatement.h"
#include "../expression/IExpression.h"

namespace c4 {
    namespace model {
        namespace statement {
            class ExpressionStatement : IStatement {
                            private:
                                std::shared_ptr<const expression::IExpression> expr;
                            public:
                                ExpressionStatement(std::shared_ptr<const expression::IExpression> a) : expr(a){};
                                std::shared_ptr<const expression::IExpression> getExpression(){return expr;}
                        };
                    }
                }
            }