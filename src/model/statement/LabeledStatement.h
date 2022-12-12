#pragma once
#include "IStatement.h"
#include "../expression/IdentifierExpression.h"
namespace c4 {
    namespace model {
        namespace statement {
            class LabeledStatement : IStatement {
                private:
                    std::shared_ptr<expression::IdentifierExpression> identifier;
                    std::shared_ptr<IStatement> statement;
                public:
                    LabeledStatement(std::shared_ptr<expression::IdentifierExpression> i, std::shared_ptr<IStatement> s) : identifier(i), statement(s){};
                    std::shared_ptr<expression::IdentifierExpression> getIdentifier(){return identifier;}
                    std::shared_ptr<IStatement> getStatement(){return statement;}                
            };
        }
    }
}
