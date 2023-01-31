#pragma once
#include "IDeclaration.h"

namespace c4 {
    namespace model {
        namespace declaration {
            class Pointer : public IDeclaration{
            private:
                std::shared_ptr<IDeclaration> ptr;                
            public:
                Pointer(std::shared_ptr<IDeclaration> ptr) : ptr(ptr){};
                std::shared_ptr<IDeclaration> getPointer(){return ptr;}
            };
        }
    }
}