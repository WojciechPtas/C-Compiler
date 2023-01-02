#pragma once
#include "IDeclaration.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class ParameterDeclaration : IDeclaration{};
            private:
               
                std::shared_ptr<Declarator> dec;
            public:
                //InitDeclartor(std::shared_ptr<InitDeclaratorList> a)
            }
        }
    }