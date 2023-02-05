#pragma once
#include "IDeclaration.h"
#include "StructUnionSpecifier.h"
#include "../token/Keyword.h"
namespace c4::model::declaration{
    class DeclarationSpecifier : public IDeclaration{
        public:
        token::Keyword keyword;
        std::shared_ptr<StructUnionSpecifier> structorunion;
        
        DeclarationSpecifier(token::Keyword keyword,
        std::shared_ptr<StructUnionSpecifier> structorunion,
        std::shared_ptr<token::Token> token) :
        IDeclaration(token), keyword(keyword), structorunion(structorunion){};
        void accept(util::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
    };
}