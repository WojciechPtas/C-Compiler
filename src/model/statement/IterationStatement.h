#pragma once
#include "IStatement.h"
#include "../expression/IExpression.h"
using namespace c4::model::expression;
namespace c4 {
    namespace model {
        namespace statement {
            class IterationStatement : IStatement {
                private:
                    std::shared_ptr<IExpression> expr;
                    std::shared_ptr<IStatement> statement;
                public:
                    IterationStatement(
                        std::shared_ptr<IExpression> expr,
                        std::shared_ptr<IStatement> statement
                    ) : expr(expr), statement(statement){};
                    std::shared_ptr<IExpression> getExpression(){return expr;}
                    std::shared_ptr<IStatement> getStatement(){return statement;}
                };
            }
        }
    }