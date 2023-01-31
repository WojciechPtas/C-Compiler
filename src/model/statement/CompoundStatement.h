#pragma once
#include <vector>
#include "IStatement.h"
namespace c4 {
    namespace model {
        namespace statement {
            class CompoundStatement : public IStatement {
                private:
                   std::vector<std::shared_ptr<IStatement>> block_of_statements;
                public:
                    CompoundStatement(std::vector<std::shared_ptr<IStatement>>& a) : block_of_statements(a){};
                    std::vector<std::shared_ptr<IStatement>>& getBlockOfStatements(){return block_of_statements;}

            };
        }
    }
}
