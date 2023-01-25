#pragma once
#include "IDeclaration.h"
#include "StructUnionSpecifier.h"
#include "../token/Keyword.h"
namespace c4::model::declaration{
    class DeclarationSpecifier : public IDeclaration{
        private:
        token::Keyword keyword;
        std::shared_ptr<StructUnionSpecifier> structorunion;
        public:
        DeclarationSpecifier(token::Keyword keyword,
        std::shared_ptr<StructUnionSpecifier> structorunion) : keyword(keyword), structorunion(structorunion){};
    }
}