#pragma once
#include "IStatement.h"
#include "../expression/IExpression.h"

namespace c4 {
    namespace model {
        namespace statement {
            class ExpressionStatement :public IStatement {
                            public:
                                std::shared_ptr<const expression::IExpression> expr;
                            
                                ExpressionStatement(std::shared_ptr<const expression::IExpression> a) : expr(a){};
                                std::shared_ptr<const expression::IExpression> getExpression(){return expr;}
                                void accept(IASTVisitor &visitor) const override {
                                    visitor.visit(*this);
                                }
                        };
                    }
                }
            }