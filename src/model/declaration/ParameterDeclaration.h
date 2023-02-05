#pragma once
#include "../token/Keyword.h"
#include "IDeclaration.h"
#include "DeclarationSpecifier.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class Declarator;
            class ParameterDeclaration :public IDeclaration{
            public:
                std::shared_ptr<DeclarationSpecifier> type;
                std::shared_ptr<Declarator> dec;            
                ParameterDeclaration(std::shared_ptr<DeclarationSpecifier> type,
                std::shared_ptr<Declarator> dec
                ) : IDeclaration(type->first_token),type(type), dec(dec) {}
                void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
            };
        }
    }
}