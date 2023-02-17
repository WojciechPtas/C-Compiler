#pragma once
#include <string>
#include "DeclarationSpecifier.h"
#include "Declarator.h"
namespace c4::model::declaration{
    class TypeName : public IDeclaration{
        public:
        std::shared_ptr<DeclarationSpecifier> ds;
        std::shared_ptr<Declarator> declarator;
       
        TypeName( std::shared_ptr<DeclarationSpecifier> ds,
        std::shared_ptr<Declarator> declarator,
        std::shared_ptr<token::Token> token=nullptr) :  IDeclaration(ds->firstTerminal), ds(ds), declarator(declarator){}
        void accept(util::IASTVisitor &visitor) const override {
                        //visitor.visit(*this);
                    }
    };
}