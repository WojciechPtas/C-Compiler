#pragma once
#include "IDeclaration.h"

namespace c4 {
    namespace model {
        namespace declaration {
            class Pointer : public IDeclaration{
            public:
                std::shared_ptr<Pointer> ptr;                
            
                Pointer(std::shared_ptr<Pointer> ptr,
                std::shared_ptr<token::Token> token) : IDeclaration(token), ptr(ptr){};
                    std::shared_ptr<IDeclaration> getPointer(){return ptr;}
                void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
            };
        }
    }
}