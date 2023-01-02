#pragma once
#include "IDeclaration.h"
#include "Pointer.h"
#include "ParameterDeclaration.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class Declarator : IDeclaration{};
            private:
                std::shared_ptr<Pointer> ptr;
                std::shared_ptr<Declarator> dec;
                std::vector<std::shared_ptr<ParameterDeclaration>> vec;
            public:
                Declarator( std::shared_ptr<Pointer> ptr, std::shared_ptr<Declarator> dec, std::vector<std::shared_ptr<ParameterDeclaration>>& vec): ptr(ptr), dec(dec), vec(vec){}
                std::shared_ptr<Pointer> getPointer(){return ptr;}
                std::shared_ptr<Declarator> getDeclaration(){return dec;}
                std::vector<std::shared_ptr<ParameterDeclaration>> getParameterDeclaration(){return vec};
                //InitDeclartor(std::shared_ptr<InitDeclaratorList> a)
            }
        }
    }