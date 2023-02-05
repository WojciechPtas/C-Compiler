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
            
                Declarator( std::shared_ptr<Pointer> ptr, std::shared_ptr<DirectDeclarator> dec,
                std::shared_ptr<token::Token> token=nullptr)
                :  IDeclaration(token), ptr(ptr), dec(dec){}
                void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
            };
        }
    }
}