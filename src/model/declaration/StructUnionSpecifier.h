#pragma once
#include <string>
#include "IDeclaration.h"

namespace c4::model::declaration{
    class StructUnionSpecifier : public IDeclaration{
        private:
        std::string name;
        std::shared_ptr<IDeclaration> declarations;
        public:
        StructUnionSpecifier(std::string name,
        std::shared_ptr<IDeclaration> declarations) : name(name), declarations(declarations){}
    };
}