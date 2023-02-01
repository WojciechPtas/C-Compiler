#pragma once
//class DeclarationSpecifier;
//class Declarator;
#include "IDeclaration.h"
namespace c4::model::declaration{
    class Declaration : public IDeclaration{
        public:
            std::shared_ptr<IDeclaration> ds;
            std::shared_ptr<IDeclaration> declarator;
        
            Declaration(std::shared_ptr<IDeclaration> ds,
            std::shared_ptr<IDeclaration> declarator) : ds(ds), declarator(declarator) {};
            void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
    };
}