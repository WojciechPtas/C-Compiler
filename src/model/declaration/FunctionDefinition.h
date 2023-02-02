#pragma once

#include "IDeclaration.h"
// #include "Declarator.h"
// #include "Pointer.h"
// #include "DeclarationSpecifier.h"
#include "../statement/CompoundStatement.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class FunctionDefinition :public IDeclaration{
           public:
                std::shared_ptr<IDeclaration> ds;
                std::shared_ptr<IDeclaration> declarator;
                std::shared_ptr<statement::CompoundStatement> statement;
                
                FunctionDefinition(
                    std::shared_ptr<IDeclaration> ds,
                    std::shared_ptr<IDeclaration> dec,
                    std::shared_ptr<statement::CompoundStatement> statement,
                    std::shared_ptr<token::Token> token=nullptr 
                    ): IDeclaration(token),ds(ds), declarator(dec), statement(statement) {};
                void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
            };
        }
    }
}