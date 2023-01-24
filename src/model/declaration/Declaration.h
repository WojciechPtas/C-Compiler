#pragma once
#include "IDeclaration.h"
#include "../token/Keyword.h"
#include "Declarator.h"
namespace c4::model::declaration{
    class Declaration : public IDeclaration{
        private:
            token::Keyword type;
            std::shared_ptr<Declarator> declarator;
        public:
            Declaration(token::Keyword type,
            std::shared_ptr<Declarator> declarator) : type(type), declarator(declarator) {};
            
    };
}