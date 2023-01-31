#pragma once

#include "IDeclaration.h"
// #include "Declarator.h"
// #include "Pointer.h"
// #include "DeclarationSpecifier.h"
#include "../statement/CompoundStatement.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class FunctionDefinition :public IDeclaration{;
            private:
                std::shared_ptr<IDeclaration> ds;
                std::shared_ptr<IDeclaration> declarator;
                std::shared_ptr<statement::CompoundStatement> statement;
            public:
                FunctionDefinition(
                    std::shared_ptr<IDeclaration> ds,
                    std::shared_ptr<IDeclaration> dec,
                    std::shared_ptr<statement::CompoundStatement> statement 
                    ):ds(ds), declarator(dec), statement(statement) {};
                std::shared_ptr<IDeclaration> getDeclaration(){return declarator;}
                std::shared_ptr<statement::CompoundStatement>  getCompoundStatement(){return statement;}
            };
        }
    }
}