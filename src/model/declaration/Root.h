#pragma once
#include "IDeclaration.h"
namespace c4::model::declaration{
    class Root: public IDeclaration{
        private:
        std::vector<IDeclaration> definitions;
        public:
        Root(std::vector<IDeclaration> definitions) :definitions(definitions) {}
    };
}