#pragma once
#include <string>
#include "StructDeclarationList.h"

namespace c4::model::declaration{
    class StructUnionSpecifier : public IDeclaration{
        private:
        std::string name;
        std::shared_ptr<StructDeclarationList> declarations;
        public:
        StructUnionSpecifier(std::string name,
        std::shared_ptr<StructDeclarationList> declarations) : name(name), declarations(declarations){}
    };
}