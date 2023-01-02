#pragma once
#include "IDeclaration.h"

namespace c4 {
    namespace model {
        namespace declaration {
            class Pointer : IDeclaration{};
            private:
                std::shared_ptr<Pointer> ptr;                
            public:
                Pointer(std::shared_ptr<Pointer> ptr) : ptr(ptr){};
                std::shared_ptr<Pointer> getPointer(){return ptr;}
            }
        }
    }