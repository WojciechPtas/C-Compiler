#pragma once
#include "IDeclaration.h"

namespace c4 {
    namespace model {
        namespace declaration {
            class Declarator : public IDeclaration{;
            public:
                std::shared_ptr<IDeclaration> ptr;
                std::shared_ptr<IDeclaration> dec;
            
                Declarator( std::shared_ptr<IDeclaration> ptr, std::shared_ptr<IDeclaration> dec)
                : ptr(ptr), dec(dec) {}
                std::shared_ptr<IDeclaration> getPointer(){return ptr;}
                std::shared_ptr<IDeclaration> getDeclaration(){return dec;}
                void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
            };
        }
    }
}