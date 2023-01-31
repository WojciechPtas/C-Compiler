#pragma once
#include "IDeclaration.h"
#include "../token/Keyword.h"
#include "Declarator.h"
#include "DeclarationSpecifier.h"

namespace c4::model::declaration{
    class Declaration : public IDeclaration{
        private:
            std::shared_ptr<DeclarationSpecifier> ds;
            std::shared_ptr<Declarator> declarator;
        public:
            Declaration(std::shared_ptr<DeclarationSpecifier> ds,
            std::shared_ptr<Declarator> declarator) : ds(ds), declarator(declarator) {};
            
    };
}