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
                        std::shared_ptr<IStatement> statement,
                        std::shared_ptr<token::Token> token
                    ) : IStatement(token),expr(expr), statement(statement){};
                    void accept(util::IASTVisitor &visitor) const override {
                    visitor.visit(*this);
                }
                };
            }
        }
    }