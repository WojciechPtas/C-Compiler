#pragma once
#include "IStatement.h"
#include "../expression/IExpression.h"
namespace c4 {
    namespace model {
        namespace statement {
            class JumpStatement : IStatement {
                private:
                    std::shared_ptr<c4::model::expression::IExpression> returnExpression;
                    std::shared_ptr<IStatement> gotoIdentifier;
                public:
                    SelectionStatement(
                        std::shared_ptr<c4::model::expression::IExpression> returnExpression,
                        std::shared_ptr<IStatement> gotoIdentifier
                    ) : returnExpression(returnExpression), gotoIdentifier(gotoIdentifier){};
                    std::shared_ptr<IExpression> getIfExpression(){return ifExpr;}
                    std::shared_ptr<IStatement> getThenStatement(){return thenStatement;}
                    std::shared_ptr<IStatement> getElseStatement(){return elseStatement;}
                };
            }
        }
    }