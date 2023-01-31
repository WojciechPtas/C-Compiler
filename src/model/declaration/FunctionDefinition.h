#pragma once

#include "IDeclaration.h"
#include "Declarator.h"
#include "Pointer.h"
#include "DeclarationSpecifier.h"
#include "../statement/CompoundStatement.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class FunctionDefinition : IDeclaration{;
            private:
                std::shared_ptr<DeclarationSpecifier> ds;
                std::shared_ptr<Declarator> declarator;
                std::shared_ptr<statement::CompoundStatement> statement;
            public:
                FunctionDefinition(
                    std::shared_ptr<DeclarationSpecifier> ds,
                    std::shared_ptr<Declarator> dec,
                    std::shared_ptr<statement::CompoundStatement> statement 
                    ):declarator(dec), statement(statement), ds(ds){};
                std::shared_ptr<Declarator> getDeclaration(){return declarator;}
                std::shared_ptr<statement::CompoundStatement>  getCompoundStatement(){return statement;}
            };
        }
    }
}