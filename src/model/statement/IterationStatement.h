#pragma once
#include "IStatement.h"
#include "../expression/IExpression.h"

namespace c4 {
    namespace model {
        namespace statement {
            class IterationStatement : public IStatement {
                private:
                    std::shared_ptr<const model::expression::IExpression> expr;
                    std::shared_ptr<IStatement> statement;
                public:
                    IterationStatement(
                        std::shared_ptr<const model::expression::IExpression> expr,
                        std::shared_ptr<IStatement> statement
                    ) : expr(expr), statement(statement){};
                    std::shared_ptr<const model::expression::IExpression> getExpression(){return expr;}
                    std::shared_ptr<IStatement> getStatement(){return statement;}
                };
            }
        }
    }