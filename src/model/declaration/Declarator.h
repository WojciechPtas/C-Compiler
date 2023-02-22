#pragma once
#include "IDeclaration.h"
#include "Pointer.h"
#include "DirectDeclarator.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class Declarator : public IDeclaration{;
            public:
                std::shared_ptr<Pointer> ptr;
                std::shared_ptr<DirectDeclarator> dec;
            
                Declarator( std::shared_ptr<Pointer> ptr, std::shared_ptr<DirectDeclarator> dec)
                :  IDeclaration(dec->firstTerminal), ptr(ptr), dec(dec){}
                void accept(util::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
            };
        }
    }
}