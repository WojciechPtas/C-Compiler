#pragma once
#include "../token/Keyword.h"
#include "IDeclaration.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class ParameterDeclaration : IDeclaration{
            private:
                token::Keyword type;
                std::shared_ptr<Declarator> dec;
            public:
                ParameterDeclaration(token::Keyword type,
                std::shared_ptr<Declarator> dec) : type(type), dec(dec) {}
            };
        }
    }
}