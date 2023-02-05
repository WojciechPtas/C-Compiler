#pragma once
#include "IStatement.h"
#include "../expression/IdentifierExpression.h"
namespace c4 {
    namespace model {
        namespace statement {
            class LabeledStatement : public IStatement {
                public:
                    std::string identifier;
                    std::shared_ptr<IStatement> statement;
                
                    LabeledStatement(std::string i, std::shared_ptr<IStatement> s,
                    std::shared_ptr<token::Token> token
                    ) :  IStatement(token), identifier(i), statement(s){};
                    std::string getIdentifier(){return identifier;}
                    std::shared_ptr<IStatement> getStatement(){return statement;}
                    void accept(util::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }                
            };
        }
    }
}
