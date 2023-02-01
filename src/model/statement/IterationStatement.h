#pragma once
#include "IStatement.h"
#include "../expression/IExpression.h"

namespace c4 {
    namespace model {
        namespace statement {
            class IterationStatement : public IStatement {
                public:
                    std::shared_ptr<const model::expression::IExpression> expr;
                    std::shared_ptr<IStatement> statement;
                
                    IterationStatement(
                        std::shared_ptr<const model::expression::IExpression> expr,
                        std::shared_ptr<IStatement> statement
                    ) : expr(expr), statement(statement){};
                    void accept(IASTVisitor &visitor) const override {
                    visitor.visit(*this);
                }
                };
            }
        }
    }