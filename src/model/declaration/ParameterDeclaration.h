#pragma once
#include "../token/Keyword.h"
#include "IDeclaration.h"

namespace c4 {
    namespace model {
        namespace declaration {
            class ParameterDeclaration :public IDeclaration{
            public:
                std::shared_ptr<IDeclaration> type;
                std::shared_ptr<IDeclaration> dec;            
                ParameterDeclaration(std::shared_ptr<IDeclaration> type,
                std::shared_ptr<IDeclaration> dec
                ) : IDeclaration(type->first_token),type(type), dec(dec) {}
                void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
            };
        }
    }
}