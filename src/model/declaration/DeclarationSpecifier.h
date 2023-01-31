#pragma once
#include "IDeclaration.h"

#include "../token/Keyword.h"
namespace c4::model::declaration{
    class DeclarationSpecifier : public IDeclaration{
        private:
        token::Keyword keyword;
        std::shared_ptr<IDeclaration> structorunion;
        public:
        DeclarationSpecifier(token::Keyword keyword,
        std::shared_ptr<IDeclaration> structorunion) : keyword(keyword), structorunion(structorunion){};
    };
}