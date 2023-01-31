#pragma once
#include <vector>

namespace c4::model::declaration{
    class StructDeclarationList: public IDeclaration{
        private:
        std::vector<std::shared_ptr<IDeclaration>> declarations;
        public:
        StructDeclarationList(std::vector<std::shared_ptr<IDeclaration>> declarations) : declarations(declarations){}
    };

}