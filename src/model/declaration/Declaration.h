#pragma once
//class DeclarationSpecifier;
//class Declarator;
#include "IDeclaration.h"
namespace c4::model::declaration{
    class Declaration : public IDeclaration{
        private:
            std::shared_ptr<IDeclaration> ds;
            std::shared_ptr<IDeclaration> declarator;
        public:
            Declaration(std::shared_ptr<IDeclaration> ds,
            std::shared_ptr<IDeclaration> declarator) : ds(ds), declarator(declarator) {};
            
    };
}