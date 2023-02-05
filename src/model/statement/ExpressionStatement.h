#pragma once
#include "IStatement.h"
#include "../expression/IExpression.h"

namespace c4 {
    namespace model {
        namespace statement {
            class ExpressionStatement :public IStatement {
                            public:
                                std::shared_ptr<const expression::IExpression> expr;
                            
                                ExpressionStatement(std::shared_ptr<const expression::IExpression> a,
                                std::shared_ptr<token::Token> token) :  IStatement(token),expr(a){};
                                void accept(util::IASTVisitor &visitor) const override {
                                    visitor.visit(*this);
                                }
                        };
                    }
                }
            }