#pragma once
#include "IDeclaration.h"

#include "../token/Keyword.h"
namespace c4::model::declaration{
    class DeclarationSpecifier : public IDeclaration{
        public:
        token::Keyword keyword;
        std::shared_ptr<IDeclaration> structorunion;
        
        DeclarationSpecifier(token::Keyword keyword,
        std::shared_ptr<IDeclaration> structorunion) : keyword(keyword), structorunion(structorunion){};
        void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
    };
}