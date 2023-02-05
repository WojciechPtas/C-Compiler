#pragma once
#include <vector>
#include "IStatement.h"
namespace c4 {
    namespace model {
        namespace statement {
            class CompoundStatement : public IStatement {
                public:
                    std::vector<std::shared_ptr<IStatement>> block_of_statements;
                
                    CompoundStatement(std::vector<std::shared_ptr<IStatement>>& a,
                    std::shared_ptr<token::Token> token) : IStatement(token), block_of_statements(a){};
                    std::vector<std::shared_ptr<IStatement>>& getBlockOfStatements(){return block_of_statements;}
                    void accept(util::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
            };
        }
    }
}
