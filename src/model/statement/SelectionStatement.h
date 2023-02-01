#pragma once
#include "IStatement.h"
#include "../expression/IExpression.h"
namespace c4 {
    namespace model {
        namespace statement {
            class SelectionStatement : public IStatement {
                public:
                    std::shared_ptr<const c4::model::expression::IExpression> ifExpr;
                    std::shared_ptr<IStatement> thenStatement;
                    std::shared_ptr<IStatement> elseStatement;
                
                    SelectionStatement(
                        std::shared_ptr<const c4::model::expression::IExpression> ifExpr,
                        std::shared_ptr<IStatement> thenStatement,
                        std::shared_ptr<IStatement> elseStatement=nullptr
                    ) : ifExpr(ifExpr), thenStatement(thenStatement), elseStatement(elseStatement) {};
                    void accept(IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
                };
            }
        }
    }