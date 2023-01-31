#pragma once
#include "IDeclaration.h"
#include "Pointer.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class Declarator : IDeclaration{;
            private:
                std::shared_ptr<Pointer> ptr;
                std::shared_ptr<Declarator> dec;
            public:
                Declarator( std::shared_ptr<Pointer> ptr, std::shared_ptr<Declarator> dec)
                : ptr(ptr), dec(dec) {}
                std::shared_ptr<Pointer> getPointer(){return ptr;}
                std::shared_ptr<Declarator> getDeclaration(){return dec;}
            };
        }
    }
}