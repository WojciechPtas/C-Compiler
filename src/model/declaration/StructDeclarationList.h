#pragma once
#include <vector>
#include "Declaration.h"
namespace c4::model::declaration{
    class StructDeclarationList: public IDeclaration{
        private:
        std::vector<std::shared_ptr<Declaration>> declarations;
        public:
        StructDeclarationList(std::vector<std::shared_ptr<Declaration>> declarations) : declarations(declarations){}
    };

}