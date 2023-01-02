#pragma once
#include "IStatement.h"
#include "../expression/IExpression.h"
namespace c4 {
    namespace model {
        namespace statement {
            class SelectionStatement : IStatement {
                private:
                    std::shared_ptr<c4::model::expression::IExpression> ifExpr;
                    std::shared_ptr<IStatement> thenStatement;
                    std::shared_ptr<IStatement> elseStatement;
                public:
                    SelectionStatement(
                        std::shared_ptr<c4::model::expression::IExpression> ifExpr,
                        std::shared_ptr<IStatement> thenStatement,
                        std::shared_ptr<IStatement> elseStatement=nullptr
                    ) : ifExpr(ifExpr), thenStatement(thenStatement), elseStatement(elseStatement) {};
                    std::shared_ptr<IExpression> getIfExpression(){return ifExpr;}
                    std::shared_ptr<IStatement> getThenStatement(){return thenStatement;}
                    std::shared_ptr<IStatement> getElseStatement(){return elseStatement;}
                };
            }
        }
    }