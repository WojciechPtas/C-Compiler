#pragma once
#include "IDeclaration.h"

namespace c4 {
    namespace model {
        namespace declaration {
            class Declarator : public IDeclaration{;
            private:
                std::shared_ptr<IDeclaration> ptr;
                std::shared_ptr<IDeclaration> dec;
            public:
                Declarator( std::shared_ptr<IDeclaration> ptr, std::shared_ptr<IDeclaration> dec)
                : ptr(ptr), dec(dec) {}
                std::shared_ptr<IDeclaration> getPointer(){return ptr;}
                std::shared_ptr<IDeclaration> getDeclaration(){return dec;}
            };
        }
    }
}