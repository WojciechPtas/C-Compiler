#pragma once
//class DeclarationSpecifier;
//class Declarator;
#include "IDeclaration.h"
#include "DeclarationSpecifier.h"
#include "Declarator.h"
namespace c4::model::declaration{
    class Declaration : public IDeclaration{
        public:
            std::shared_ptr<DeclarationSpecifier> ds;
            std::shared_ptr<Declarator> declarator;
        
            Declaration(std::shared_ptr<DeclarationSpecifier> ds,
            std::shared_ptr<Declarator> declarator) :IDeclaration(ds->first_token), ds(ds),
             declarator(declarator){};
            void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
    };
}